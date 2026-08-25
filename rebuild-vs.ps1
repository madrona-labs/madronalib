<#
.SYNOPSIS
    Fresh build and install of madronalib for the Windows / Visual Studio workflow.

.DESCRIPTION
    The Windows counterpart of ./rebuild-xcode. Wipes the build tree, regenerates
    build/madronalib.sln, clears the installed library out of the install prefix,
    then builds and installs the madronalib target for each configuration.

    Installing into C:\Program Files needs administrator rights, so the script
    relaunches itself elevated when the install prefix turns out not to be
    writable. That is the equivalent of the sudo calls in rebuild-xcode. Point
    -InstallPrefix at somewhere you own and no elevation happens at all.

.EXAMPLE
    .\rebuild-vs.cmd
    Full rebuild and install of Debug and Release.

.EXAMPLE
    .\rebuild-vs.cmd -Configs Release -Open
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

function Test-Administrator {
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($identity)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
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
# elevate if needed
#--------------------------------------------------------------------

if (-not (Test-Administrator) -and -not (Test-PathWritable $InstallPrefix)) {
    Write-Host "Installing to '$InstallPrefix' needs administrator rights - relaunching elevated." -ForegroundColor Yellow

    # Forward only this script's own parameters; the common parameters that
    # CmdletBinding adds are not ours to pass along.
    $ownParams = @('Generator', 'Platform', 'InstallPrefix', 'Configs', 'Open')
    $forwarded = @()
    foreach ($name in $ownParams) {
        if (-not $PSBoundParameters.ContainsKey($name)) { continue }
        $value = $PSBoundParameters[$name]
        if ($value -is [switch]) {
            if ($value.IsPresent) { $forwarded += "-$name" }
        }
        elseif ($value -is [array]) {
            $forwarded += "-$name"
            $forwarded += ($value -join ',')
        }
        else {
            $forwarded += "-$name"
            $forwarded += """$value"""
        }
    }

    $hostExe = (Get-Process -Id $PID).Path
    if (-not $hostExe) { $hostExe = 'powershell.exe' }

    $argList = @('-NoExit', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', """$PSCommandPath""") + $forwarded
    Start-Process -FilePath $hostExe -Verb RunAs -ArgumentList $argList
    exit 0
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
