#!/usr/bin/env pwsh
<#
.SYNOPSIS
Migrates unit test projects from MOCK_PRECOMPILE_HEADERS to ENABLE_TEST_FILES_PRECOMPILED_HEADERS

.DESCRIPTION
This script processes unit test projects, extracts includes from _ut.c files into separate PCH headers,
moves MOCKABLE_FUNCTION sections to proper locations, and updates CMakeLists.txt files accordingly.

.PARAMETER TestMode
Process only a single test project for validation (default: $false)

.PARAMETER ProjectPath
Specific project path to process when TestMode is enabled
#>

param(
    [switch]$TestMode = $false,
    [string]$ProjectPath = ""
)

# Set error action preference
$ErrorActionPreference = "Stop"

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    Write-Host "[$timestamp] [$Level] $Message"
}

function Find-UnitTestProjects {
    param([string]$RootPath)
    
    Write-Log "Finding unit test projects..."
    $projects = @()
    
    # Find all _ut.c files excluding deps folder
    $utFiles = Get-ChildItem -Path $RootPath -Recurse -Filter "*_ut.c" | Where-Object {
        $_.DirectoryName -notlike "*\deps\*"
    }
    
    foreach ($utFile in $utFiles) {
        $projectDir = $utFile.Directory
        $cmakeFile = Join-Path $projectDir "CMakeLists.txt"
        
        if (Test-Path $cmakeFile) {
            $projects += @{
                ProjectDir = $projectDir.FullName
                UtFile = $utFile.FullName
                CmakeFile = $cmakeFile
                TestName = [System.IO.Path]::GetFileNameWithoutExtension($utFile.Name)
            }
        }
    }
    
    Write-Log "Found $($projects.Count) unit test projects"
    return $projects
}

function Parse-IncludeSection {
    param([string]$FilePath)
    
    Write-Log "Parsing include section from: $FilePath"
    
    $lines = Get-Content -Path $FilePath
    
    # Step 1: Find the last #include line
    $lastIncludeIndex = -1
    for ($i = 0; $i -lt $lines.Count; $i++) {
        $trimmedLine = $lines[$i].Trim()
        if ($trimmedLine -match '^#include\s+[<"]') {
            $lastIncludeIndex = $i
        }
    }
    
    if ($lastIncludeIndex -eq -1) {
        Write-Log "No #include statements found in file" -Level "WARNING"
        return @{
            IncludeLines = @()
            MockableFunctions = @()
            AfterIncludesLines = $lines
        }
    }
    
    Write-Log "Last #include found at line $($lastIncludeIndex + 1)"
    
    # Step 2: Process lines up to and including the last #include
    $includeLines = @()
    $mockableFunctions = @()
    $afterIncludesLines = @()
    
    # Track state for processing
    $inMockableFunction = $false
    $inMockFunctionWithCode = $false
    $mockableFunctionBuffer = @()
    
    for ($i = 0; $i -le $lastIncludeIndex; $i++) {
        $line = $lines[$i]
        $trimmedLine = $line.Trim()
        
        # Handle MOCKABLE_FUNCTION blocks - these should be left in the UT file
        if ($trimmedLine -match '^MOCKABLE_FUNCTION') {
            $inMockableFunction = $true
            $mockableFunctionBuffer = @($line)
            # Check if this is a single-line MOCKABLE_FUNCTION
            if ($trimmedLine -match '\);?\s*$') {
                $mockableFunctions += $mockableFunctionBuffer -join "`r`n"
                $mockableFunctionBuffer = @()
                $inMockableFunction = $false
            }
            continue
        }
        
        if ($inMockableFunction) {
            $mockableFunctionBuffer += $line
            # For MOCKABLE_FUNCTION, look for the closing parenthesis and semicolon
            if ($trimmedLine -match '\);?\s*$') {
                $mockableFunctions += $mockableFunctionBuffer -join "`r`n"
                $mockableFunctionBuffer = @()
                $inMockableFunction = $false
            }
            continue
        }
        
        # Handle MOCK_FUNCTION_WITH_CODE blocks - these should be left in the UT file
        if ($trimmedLine -match '^MOCK_FUNCTION_WITH_CODE\(') {
            $inMockFunctionWithCode = $true
            $mockableFunctionBuffer = @($line)
            continue
        }
        
        if ($inMockFunctionWithCode) {
            $mockableFunctionBuffer += $line
            if ($trimmedLine -match '^MOCK_FUNCTION_END\(') {
                $mockableFunctions += $mockableFunctionBuffer -join "`r`n"
                $mockableFunctionBuffer = @()
                $inMockFunctionWithCode = $false
            }
            continue
        }
        
        # Check if this line should be left in the UT file (not an #include/#ifdef/#define/#else/#endif)
        $isIncludeRelated = $trimmedLine -match '^#(include|ifdef|ifndef|define|undef|if|else|elif|endif)' -or
                           $trimmedLine -eq '' -or
                           $trimmedLine.StartsWith('//') -or
                           $trimmedLine.StartsWith('/*')
        
        if ($isIncludeRelated) {
            # Don't include .c files in PCH headers - they contain implementations
            # Keep them as includes in the UT file instead, wrapped in ENABLE_MOCKS
            if ($trimmedLine -match '#include\s+"[^"]*\.c"' -or $trimmedLine -match "#include\s+'[^']*\.c'") {
                # This is a .c file include - it should stay in the UT file
                $afterIncludesLines += $line
            } else {
                $includeLines += $line
            }
        } else {
            # This line is not include-related, leave it in the UT file
            $afterIncludesLines += $line
        }
    }
    
    # Add all remaining lines after the last include to afterIncludesLines
    for ($i = $lastIncludeIndex + 1; $i -lt $lines.Count; $i++) {
        $afterIncludesLines += $lines[$i]
    }
    
    # Remove trailing empty lines from include section
    while ($includeLines.Count -gt 0 -and $includeLines[-1].Trim() -eq '') {
        $includeLines = $includeLines[0..($includeLines.Count - 2)]
    }
    
    return @{
        IncludeLines = $includeLines
        MockableFunctions = $mockableFunctions
        AfterIncludesLines = $afterIncludesLines
    }
}

function Create-PchHeader {
    param(
        [string]$ProjectDir,
        [string]$TestName,
        [string[]]$IncludeLines,
        [string]$UtFilePath
    )
    
    $pchFileName = "${TestName}_pch.h"
    $pchFilePath = Join-Path $ProjectDir $pchFileName
    
    Write-Log "Creating PCH header: $pchFilePath"
    
    # Extract original copyright notice from the UT file
    $originalLines = Get-Content -Path $UtFilePath
    $copyrightLines = @()
    
    foreach ($line in $originalLines) {
        $trimmedLine = $line.Trim()
        if ($trimmedLine.StartsWith('// Copyright') -or 
            $trimmedLine.StartsWith('// Licensed under') -or 
            ($copyrightLines.Count -gt 0 -and $trimmedLine -eq '')) {
            $copyrightLines += $line
            if ($trimmedLine.StartsWith('// Licensed under')) {
                break
            }
        } elseif ($copyrightLines.Count -gt 0) {
            break
        }
    }
    
    # Clean the include lines to remove all copyright/license lines
    $cleanIncludeLines = @()
    $skipCopyrightSection = $true
    
    foreach ($line in $IncludeLines) {
        $trimmedLine = $line.Trim()
        
        # Skip copyright and license lines at the beginning
        if ($skipCopyrightSection) {
            if ($trimmedLine.StartsWith('// Copyright') -or 
                $trimmedLine.StartsWith('// Licensed under') -or 
                $trimmedLine -eq '') {
                continue
            } else {
                $skipCopyrightSection = $false
            }
        }
        
        $cleanIncludeLines += $line
    }
    
    # Build PCH content with original copyright and cleaned includes
    $pchContent = ($copyrightLines -join "`r`n") + "`r`n`r`n// Precompiled header for $TestName`r`n`r`n" + ($cleanIncludeLines -join "`r`n")
    
    # Ensure the content ends with a newline
    if (-not $pchContent.EndsWith("`r`n")) {
        $pchContent += "`r`n"
    }
    
    # Write with Windows line endings (CRLF)
    [System.IO.File]::WriteAllText($pchFilePath, $pchContent, [System.Text.Encoding]::UTF8)
    
    return $pchFileName
}

function Update-UtFile {
    param(
        [string]$UtFilePath,
        [string]$PchFileName,
        [string[]]$MockableFunctions,
        [string[]]$AfterIncludesLines
    )
    
    Write-Log "Updating UT file: $UtFilePath"
    
    # Extract original copyright notice from the UT file
    $originalLines = Get-Content -Path $UtFilePath
    $copyrightLines = @()
    
    foreach ($line in $originalLines) {
        $trimmedLine = $line.Trim()
        if ($trimmedLine.StartsWith('// Copyright') -or 
            $trimmedLine.StartsWith('// Licensed under') -or 
            ($copyrightLines.Count -gt 0 -and $trimmedLine -eq '')) {
            $copyrightLines += $line
            if ($trimmedLine.StartsWith('// Licensed under')) {
                break
            }
        } elseif ($copyrightLines.Count -gt 0) {
            break
        }
    }
    
    # Build new content starting with original copyright with Windows line endings
    $newContent = ($copyrightLines -join "`r`n") + "`r`n`r`n#include `"$PchFileName`"`r`n`r`n"
    
    # Handle .c file includes that were left in afterIncludesLines
    $cFileIncludes = @()
    $filteredAfterIncludesLines = @()
    
    foreach ($line in $AfterIncludesLines) {
        $trimmedLine = $line.Trim()
        if ($trimmedLine -match '#include\s+"[^"]*\.c"' -or $trimmedLine -match "#include\s+'[^']*\.c'") {
            $cFileIncludes += $line
        } else {
            $filteredAfterIncludesLines += $line
        }
    }
    
    # Add .c file includes in ENABLE_MOCKS section
    if ($cFileIncludes.Count -gt 0) {
        $newContent += "#define ENABLE_MOCKS`r`n"
        $newContent += "#undef ENABLE_MOCKS_DECL`r`n"
        $newContent += "#include `"umock_c/umock_c_prod.h`"`r`n"
        $newContent += ($cFileIncludes -join "`r`n") + "`r`n"
        $newContent += "#undef ENABLE_MOCKS`r`n`r`n"
    }
    
    if ($MockableFunctions.Count -gt 0) {
        $newContent += "#define ENABLE_MOCKS`r`n#undef ENABLE_MOCKS_DECL`r`n#include `"umock_c/umock_c_prod.h`"`r`n" + ($MockableFunctions -join "`r`n`r`n") + "`r`n#undef ENABLE_MOCKS`r`n`r`n"
    }
    
    $newContent += ($filteredAfterIncludesLines -join "`r`n")
    
    # Write with Windows line endings (CRLF)
    [System.IO.File]::WriteAllText($UtFilePath, $newContent, [System.Text.Encoding]::UTF8)
}

function Update-CmakeFile {
    param(
        [string]$CmakeFilePath,
        [string]$TestName,
        [string]$PchFileName
    )
    
    Write-Log "Updating CMake file: $CmakeFilePath"
    
    $content = Get-Content -Path $CmakeFilePath -Raw
    
    # Pattern to match build_test_artifacts calls (both single-line and multi-line)
    $pattern = 'build_test_artifacts\([^)]+\)'
    
    if ($content -match $pattern) {
        $fullMatch = $matches[0]
        $buildTestArtifactsCall = $fullMatch
        
        # Check if the call already contains ENABLE_TEST_FILES_PRECOMPILED_HEADERS
        if ($buildTestArtifactsCall -match 'ENABLE_TEST_FILES_PRECOMPILED_HEADERS') {
            Write-Log "CMakeLists.txt already contains ENABLE_TEST_FILES_PRECOMPILED_HEADERS, skipping: $TestName" -Level "INFO"
            return
        }
        
        # Remove any existing MOCK_PRECOMPILE_HEADERS or NO_MOCK_PRECOMPILE_HEADERS and their associated header files
        $cleanedCall = $buildTestArtifactsCall -replace '\s+MOCK_PRECOMPILE_HEADERS(\s+"[^"]*")?' -replace '\s+NO_MOCK_PRECOMPILE_HEADERS'
        
        # Check if this ends with a closing parenthesis (needs to be replaced)
        if ($cleanedCall -match '\)$') {
            # Replace the closing ) with the new parameter on a new line and )
            $newCall = $cleanedCall -replace '\)$', "`r`n    ENABLE_TEST_FILES_PRECOMPILED_HEADERS `"`${CMAKE_CURRENT_LIST_DIR}/$PchFileName`")"
        } else {
            # This shouldn't happen, but handle it gracefully
            $newCall = $cleanedCall + "`r`n    ENABLE_TEST_FILES_PRECOMPILED_HEADERS `"`${CMAKE_CURRENT_LIST_DIR}/$PchFileName`""
        }
        
        # Replace the old call with the new one
        $newContent = $content -replace [regex]::Escape($fullMatch), $newCall
        
        # Write with Windows line endings (CRLF)
        [System.IO.File]::WriteAllText($CmakeFilePath, $newContent, [System.Text.Encoding]::UTF8)
    } else {
        Write-Log "Could not find build_test_artifacts call in: $CmakeFilePath" -Level "WARNING"
    }
}

function Process-Project {
    param([hashtable]$Project)
    
    Write-Log "Processing project: $($Project.TestName)"
    
    try {
        # Check if file has already been processed (contains PCH include)
        $fileContent = Get-Content -Path $Project.UtFile -Raw
        $expectedPchInclude = "#include ""$($Project.TestName)_pch.h"""
        
        if ($fileContent -match [regex]::Escape($expectedPchInclude)) {
            Write-Log "File already processed (contains PCH include): $($Project.TestName)" -Level "INFO"
            return $true
        }
        
        # Parse the UT file
        $parsed = Parse-IncludeSection -FilePath $Project.UtFile
        
        # Create PCH header
        $pchFileName = Create-PchHeader -ProjectDir $Project.ProjectDir -TestName $Project.TestName -IncludeLines $parsed.IncludeLines -UtFilePath $Project.UtFile
        
        # Update UT file
        Update-UtFile -UtFilePath $Project.UtFile -PchFileName $pchFileName -MockableFunctions $parsed.MockableFunctions -AfterIncludesLines $parsed.AfterIncludesLines
        
        # Update CMake file
        Update-CmakeFile -CmakeFilePath $Project.CmakeFile -TestName $Project.TestName -PchFileName $pchFileName
        
        Write-Log "Successfully processed: $($Project.TestName)" -Level "SUCCESS"
        return $true
    }
    catch {
        Write-Log "Failed to process $($Project.TestName): $($_.Exception.Message)" -Level "ERROR"
        return $false
    }
}

# Main execution
try {
    Write-Log "Starting PCH migration script..."
    
    if ($TestMode) {
        if ($ProjectPath -eq "") {
            Write-Log "TestMode enabled but no ProjectPath specified. Using first project found."
            $projects = Find-UnitTestProjects -RootPath "."
            if ($projects.Count -eq 0) {
                throw "No unit test projects found"
            }
            $projectToProcess = $projects[0]
        } else {
            $utFile = Get-ChildItem -Path $ProjectPath -Filter "*_ut.c" | Select-Object -First 1
            if (-not $utFile) {
                throw "No _ut.c file found in specified project path: $ProjectPath"
            }
            
            $projectToProcess = @{
                ProjectDir = $ProjectPath
                UtFile = $utFile.FullName
                CmakeFile = Join-Path $ProjectPath "CMakeLists.txt"
                TestName = [System.IO.Path]::GetFileNameWithoutExtension($utFile.Name)
            }
        }
        
        Write-Log "Test mode: Processing single project: $($projectToProcess.TestName)"
        $success = Process-Project -Project $projectToProcess
        
        if ($success) {
            Write-Log "Test completed successfully!" -Level "SUCCESS"
        } else {
            Write-Log "Test failed!" -Level "ERROR"
            exit 1
        }
    } else {
        $projects = Find-UnitTestProjects -RootPath "."
        $successCount = 0
        $failureCount = 0
        
        foreach ($project in $projects) {
            if (Process-Project -Project $project) {
                $successCount++
            } else {
                $failureCount++
            }
        }
        
        Write-Log "Migration completed: $successCount succeeded, $failureCount failed" -Level "SUCCESS"
    }
}
catch {
    Write-Log "Script failed: $($_.Exception.Message)" -Level "ERROR"
    exit 1
}