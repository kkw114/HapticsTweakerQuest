# This script compiles the mod into a .so library.
# Uses the Android Studio bundled CMake 3.22.1 + Ninja and the NDK from ndkpath.txt.

Param(
    [Parameter(Mandatory = $false)]
    [Switch] $clean,

    [Parameter(Mandatory = $false)]
    [Switch] $help
)

if ($help) {
    Write-Output "Build - Compiles the mod into a .so library"
    Write-Output "-- Arguments --"
    Write-Output "-Clean   Deletes the build folder, so that the entire library is rebuilt"
    exit
}

# CMake 的警告会写入 stderr，避免被 PowerShell 当作错误处理
$PSNativeCommandUseErrorActionPreference = $false

$CMake = "D:\AndroidStudio\Sdk\cmake\3.22.1\bin\cmake.exe"
$Ninja = "D:\AndroidStudio\Sdk\cmake\3.22.1\bin\ninja.exe"

if (-not (Test-Path $CMake)) {
    Write-Error "CMake not found: $CMake"
    exit 1
}
if (-not (Test-Path $Ninja)) {
    Write-Error "Ninja not found: $Ninja"
    exit 1
}

$Root = Split-Path -Parent $PSScriptRoot
Set-Location $Root

if ($clean -and (Test-Path "build")) {
    Remove-Item "build" -Recurse -Force
}

if (-not (Test-Path "build")) {
    New-Item -Path "build" -ItemType Directory | Out-Null
}

# 注意：构建路径必须为纯英文（中文路径会导致 CMake post-build 脚本编码错误）
& $CMake -G "Ninja" -DCMAKE_MAKE_PROGRAM="$Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -B build | Out-Host
if (-not (Test-Path "build\build.ninja")) {
    Write-Error "CMake configure failed"
    exit 1
}

& $CMake --build build | Out-Host
if (-not (Test-Path "build\libhapticstweaker.so")) {
    Write-Error "Build failed: libhapticstweaker.so not produced"
    exit 1
}

Write-Output "Build OK: build/libhapticstweaker.so"
exit 0
