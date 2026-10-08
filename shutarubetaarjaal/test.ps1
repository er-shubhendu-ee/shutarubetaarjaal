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

Write-Host "Testing SHUTARUBETAARJAAL - $Config"

cmake -S . -B build -DTEST=ON
cmake --build build --config $Config
ctest --test-dir build --build-config $Config --output-on-failure