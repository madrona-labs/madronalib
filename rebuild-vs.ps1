<#
.SYNOPSIS
    Fresh build and install of madronalib for the Windows / Visual Studio workflow.

.DESCRIPTION
    The Windows counterpart of ./rebuild-xcode. Wipes the build tree, regenerates
    build/madronalib.sln, clears the installed library out of the install prefix,
    then builds and installs the madronalib target for each configuration.

    Run it from a Windows Terminal tab and all of that happens in that tab's own
    PowerShell session - no wrapper script, no second window. A stock Windows
    install blocks .ps1 files, so allow local scripts once. That writes to your
    own user hive and needs no administrator rights:

        Set-ExecutionPolicy -Scope CurrentUser RemoteSigned

    RemoteSigned still runs unsigned local scripts; a clone carries no
    mark-of-the-web. Only a copy unzipped from a download needs Unblock-File.

    Installing into C:\Program Files needs administrator rights, and this script
    will not elevate itself: UAC cannot elevate a process that is already
    running, so doing that would mean a second window with your build output in
    it. Instead it stops before touching anything and names your options.

.EXAMPLE
    .\rebuild-vs.ps1
    Full rebuild and install of Debug and Release.

.EXAMPLE
    .\rebuild-vs.ps1 -Configs Release -Open
    Rebuild Release only, then open the solution in Visual Studio.
#>

[CmdletBinding()]
param(
    [string]   $Generator     = 'Visual Studio 17 2022',
    [string]   $Platform      = 'x64',
    [string]   $InstallPrefix = 'C:/Program Files/madronalib',
    [string[]] $Configs       = @('Debug', 'Release'),
    [switch]   $Open
)

$ErrorActionPreference = 'Stop'

#--------------------------------------------------------------------
# helpers
#--------------------------------------------------------------------

function Write-Step {
    param([string] $Message)
    Write-Host ''
    Write-Host "==> $Message" -ForegroundColor Cyan
}

function Invoke-Checked {
    param(
        [string]   $Exe,
        [string[]] $Arguments
    )
    Write-Host "    $Exe $($Arguments -join ' ')" -ForegroundColor DarkGray

    # cmake writes its message() output to stderr, and with ErrorActionPreference
    # Stop, Windows PowerShell turns each of those lines into a terminating error
    # as soon as the caller redirects the stream - so piping this script to a log
    # would kill it on the first ordinary status line. The exit code below is the
    # real check.
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
        & $Exe @Arguments
    }
    finally {
        $ErrorActionPreference = $previous
    }

    if ($LASTEXITCODE -ne 0) {
        throw "$Exe exited with code $LASTEXITCODE"
    }
}

# Can we create files where the install would put them? Walks up to the nearest
# directory that exists, since that is where the install has to start writing.
function Test-PathWritable {
    param([string] $Path)

    $probe = $Path
    while ($probe -and -not (Test-Path -LiteralPath $probe)) {
        $probe = Split-Path -Parent $probe
    }
    if (-not $probe) { return $false }

    $testFile = Join-Path $probe ([IO.Path]::GetRandomFileName())
    try {
        [IO.File]::WriteAllText($testFile, '')
        Remove-Item -LiteralPath $testFile -Force
        return $true
    }
    catch {
        return $false
    }
}

# Read the CRT that a static library asks the linker for, from the /DEFAULTLIB
# directives its member objects carry. LIBCMT = /MT, LIBCMTD = /MTd,
# MSVCRT = /MD, MSVCRTD = /MDd.
function Get-CrtDirective {
    param([string] $LibPath)

    $bytes = [IO.File]::ReadAllBytes($LibPath)
    $text = [Text.Encoding]::ASCII.GetString($bytes)
    $hits = [regex]::Matches($text, 'DEFAULTLIB:"?(LIBCMTD?|MSVCRTD?)"?')

    return @($hits | ForEach-Object { $_.Groups[1].Value.ToUpperInvariant() } | Sort-Object -Unique)
}

#--------------------------------------------------------------------
# check we can install before spending a build on it
#--------------------------------------------------------------------

# Test-Path is not the question: the prefix can exist and still refuse your
# writes, which is exactly C:\Program Files from an ordinary tab.
if (-not (Test-PathWritable $InstallPrefix)) {
    # Windows form for the commands below, since they are meant to be pasted.
    $prefixDisplay = $InstallPrefix -replace '/', '\'

    Write-Host ''
    Write-Host "Cannot write to '$prefixDisplay', so the install would fail. Three ways to fix that:" -ForegroundColor Yellow
    Write-Host ''
    Write-Host '  1. Run this tab elevated. In Windows Terminal, ctrl+shift+click the profile in'
    Write-Host '     the dropdown, or give that profile "elevate": true in settings.json.'
    Write-Host ''
    Write-Host '  2. Grant yourself write access to the prefix once, from an elevated tab, after'
    Write-Host '     which no run needs elevation at all:'
    Write-Host ''
    Write-Host "         New-Item -ItemType Directory -Force '$prefixDisplay'"
    Write-Host "         icacls '$prefixDisplay' /grant '${env:USERNAME}:(OI)(CI)M'"
    Write-Host ''
    Write-Host '  3. Install somewhere you already own:'
    Write-Host ''
    Write-Host '         .\rebuild-vs.ps1 -InstallPrefix "$env:LOCALAPPDATA\madronalib"'
    Write-Host ''
    Write-Host '     That is off the path CMake searches by default, so a downstream'
    Write-Host '     find_package(madronalib) then needs CMAKE_PREFIX_PATH pointed at it.'
    Write-Host ''
    exit 1
}

#--------------------------------------------------------------------
# paths
#--------------------------------------------------------------------

$madronalibRoot = $PSScriptRoot
$buildDir = Join-Path $madronalibRoot 'build'
$libDir = Join-Path $InstallPrefix 'lib'
$started = Get-Date

Write-Host "madronalib root : $madronalibRoot"
Write-Host "build directory : $buildDir"
Write-Host "install prefix  : $InstallPrefix"
Write-Host "generator       : $Generator ($Platform)"
Write-Host "configurations  : $($Configs -join ', ')"

#--------------------------------------------------------------------
# wipe the build tree
#--------------------------------------------------------------------

if (Test-Path -LiteralPath $buildDir) {
    # .vs holds Visual Studio's own file index rather than any build output, and
    # VS keeps it locked for as long as the solution is open. Leaving it in place
    # lets the rebuild run without making you close the IDE first - which also
    # means a run that died after the wipe can leave .vs as the only thing here.
    $contents = @(Get-ChildItem -LiteralPath $buildDir -Force)
    $buildOutput = @($contents | Where-Object { $_.Name -ne '.vs' })

    # CMakeFiles counts as well as CMakeCache.txt: a configure that dies partway
    # leaves the former without the latter, and refusing to clean that up would
    # wedge every later run.
    $cmakeMarkers = @('CMakeCache.txt', 'CMakeFiles')
    $isBuildTree = @($cmakeMarkers | Where-Object { Test-Path -LiteralPath (Join-Path $buildDir $_) }).Count -gt 0

    if ($buildOutput.Count -gt 0 -and -not $isBuildTree) {
        throw "'$buildDir' holds files but no CMakeCache.txt or CMakeFiles, so it may not be a CMake build tree. Refusing to delete it."
    }

    Write-Step "Removing the old build tree"

    $locked = @()
    foreach ($item in $buildOutput) {
        try {
            Remove-Item -LiteralPath $item.FullName -Recurse -Force -ErrorAction Stop
        }
        catch {
            $locked += $item.FullName
        }
    }

    if ($locked.Count -gt 0) {
        throw "Could not clear the build tree. Close Visual Studio and stop any running examples, then try again. Still locked: " + ($locked -join '; ')
    }
}

#--------------------------------------------------------------------
# configure - this is also what regenerates madronalib.sln
#--------------------------------------------------------------------

Write-Step "Configuring"
Invoke-Checked 'cmake' @(
    '-S', $madronalibRoot
    '-B', $buildDir
    '-G', $Generator
    '-A', $Platform
    "-DCMAKE_INSTALL_PREFIX=$InstallPrefix"
)

#--------------------------------------------------------------------
# clean installed files
#--------------------------------------------------------------------

Write-Step "Clearing the installed library"
$installedPaths = @(
    (Join-Path $InstallPrefix 'include')
    (Join-Path $libDir 'madrona.lib')
    (Join-Path $libDir 'madrona-debug.lib')
    (Join-Path $libDir 'cmake\madronalib')
)
foreach ($path in $installedPaths) {
    if (Test-Path -LiteralPath $path) {
        Write-Host "    removing $path" -ForegroundColor DarkGray
        Remove-Item -LiteralPath $path -Recurse -Force
    }
}

#--------------------------------------------------------------------
# build and install each configuration
#--------------------------------------------------------------------

foreach ($config in $Configs) {
    Write-Step "Building $config"
    Invoke-Checked 'cmake' @('--build', $buildDir, '--config', $config, '--target', 'madronalib')

    Write-Step "Installing $config"
    Invoke-Checked 'cmake' @('--install', $buildDir, '--config', $config, '--component', 'library')
}

#--------------------------------------------------------------------
# verify the installed libraries use the static CRT
#--------------------------------------------------------------------

# A library built /MD drops MSVCRT into a plugin that is otherwise /MT, which
# either fails to link on duplicate CRT symbols or leaves the shipped binary
# needing the VC++ redistributable. That is exactly what happens when a
# configuration is missing from the /MT generator expression in CMakeLists.txt
# and quietly falls through to CMake's default. Cheap to check, expensive to miss.
Write-Step "Checking the C runtime"

$expectedRuntime = @{
    'Debug'   = 'MultiThreadedDebug'
    'Release' = 'MultiThreaded'
}
$expectedCrt = @{
    'Debug'   = 'LIBCMTD'
    'Release' = 'LIBCMT'
}
$libForConfig = @{
    'Debug'   = 'madrona-debug.lib'
    'Release' = 'madrona.lib'
}
$crtProblems = @()

# The project file is the authoritative record of what CMake configured, and it
# is readable whatever the codegen settings are.
$projectText = Get-Content -LiteralPath (Join-Path $buildDir 'madronalib.vcxproj') -Raw
$dollar = '$'

foreach ($config in $Configs) {
    if (-not $expectedRuntime.ContainsKey($config)) {
        Write-Host "    $config - no expected runtime recorded, skipping" -ForegroundColor DarkGray
        continue
    }

    $want = $expectedRuntime[$config]
    # Anchor on the ItemDefinitionGroup specifically. The same condition string
    # also appears on PropertyGroups earlier in the file.
    $condition = "'$dollar(Configuration)|$dollar(Platform)'=='$config|$Platform'"
    $marker = '<ItemDefinitionGroup Condition="' + $condition + '">'
    $start = $projectText.IndexOf($marker)

    if ($start -lt 0) {
        $crtProblems += "the project has no $config|$Platform configuration"
        continue
    }

    $end = $projectText.IndexOf('</ItemDefinitionGroup>', $start)
    $block = $projectText.Substring($start, $end - $start)
    $runtime = [regex]::Match($block, '<RuntimeLibrary>([^<]+)</RuntimeLibrary>')

    if (-not $runtime.Success) {
        $crtProblems += "$config sets no RuntimeLibrary, so it uses the CMake default rather than $want"
    }
    elseif ($runtime.Groups[1].Value -ne $want) {
        $crtProblems += "$config compiles as $($runtime.Groups[1].Value) but should be $want"
        Write-Host "    $config -> $($runtime.Groups[1].Value) (expected $want)" -ForegroundColor Red
    }
    else {
        Write-Host "    $config -> $want" -ForegroundColor Green
    }

    # Corroborate against the archive itself. A /GL build stores IL objects that
    # carry no directives at all, so finding none here is not a failure.
    $libPath = Join-Path $libDir $libForConfig[$config]
    if (Test-Path -LiteralPath $libPath) {
        $found = @(Get-CrtDirective $libPath)
        if ($found.Count -eq 0) {
            Write-Host "        $($libForConfig[$config]) carries no /DEFAULTLIB directives, as a /GL build does not" -ForegroundColor DarkGray
        }
        elseif ($found.Count -eq 1 -and $found[0] -eq $expectedCrt[$config]) {
            Write-Host "        $($libForConfig[$config]) asks the linker for $($found[0])" -ForegroundColor DarkGray
        }
        else {
            $crtProblems += "$($libForConfig[$config]) asks the linker for $($found -join ', ') but should ask for $($expectedCrt[$config])"
        }
    }
}

if ($crtProblems.Count -gt 0) {
    throw "Wrong C runtime: " + ($crtProblems -join '; ')
}

#--------------------------------------------------------------------
# done
#--------------------------------------------------------------------

$solution = Join-Path $buildDir 'madronalib.sln'
$elapsed = (Get-Date) - $started

Write-Step ("Done in {0:mm}:{0:ss}." -f $elapsed)
Write-Host "Installed to    : $InstallPrefix"
Write-Host "Solution        : $solution"

if ($Open) {
    Write-Host ''
    Write-Host 'Opening the solution...'
    Invoke-Item -LiteralPath $solution
}
