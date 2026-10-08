param(
    [switch]$D,
    [switch]$R
)

$ErrorActionPreference = "Stop"

if ($D -and $R) {
    throw "Use either -D or -R, not both."
}

if ($D) {
    $Config = "Debug"
}
else {
    $Config = "Release"
}

Write-Host "Building SHUTARUBETAARJAAL - $Config"

cmake -S . -B build -DTEST=OFF
cmake --build build --config $Config