# Copies the built .so to the Quest device via adb.
# Usage: ./scripts/copy.ps1
Param(
    [Parameter(Mandatory = $false)]
    [string] $Device = ""
)

$Root = Split-Path -Parent $PSScriptRoot
$So = Join-Path $Root "build\libhapticstweaker.so"

if (-not (Test-Path $So)) {
    Write-Error "Not found: $So — run ./scripts/build.ps1 first"
    exit 1
}

$Adb = "D:\AndroidStudio\Sdk\platform-tools\adb.exe"
if (-not (Test-Path $Adb)) {
    Write-Error "adb not found: $Adb"
    exit 1
}

$adbArgs = @("push", $So, "/sdcard/ModData/com.beatgames.beatsaber/Modloader/mods/libhapticstweaker.so")
if ($Device) {
    $adbArgs = @("-s", $Device) + $adbArgs
}

& $Adb @adbArgs
exit $LASTEXITCODE
