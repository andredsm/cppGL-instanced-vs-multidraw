# OpenGL Renderer - Build Script for Windows PowerShell
# This script installs Conan dependencies and builds the project

param(
    [switch]$Debug,
    [switch]$Clean,
    [switch]$Help
)

# Color functions for output
function Write-Status {
    param([string]$Message)
    Write-Host "[INFO] $Message" -ForegroundColor Green
}

function Write-Warning {
    param([string]$Message)
    Write-Host "[WARNING] $Message" -ForegroundColor Yellow
}

function Write-Error {
    param([string]$Message)
    Write-Host "[ERROR] $Message" -ForegroundColor Red
}

function Show-Help {
    Write-Host "OpenGL Renderer - Build Script for Windows" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Usage: .\build.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Debug    Build in Debug mode (default is Release)"
    Write-Host "  -Clean    Clean build directory before building"
    Write-Host "  -Help     Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\build.ps1              # Build in Release mode"
    Write-Host "  .\build.ps1 -Debug       # Build in Debug mode"
    Write-Host "  .\build.ps1 -Clean       # Clean and build"
    Write-Host ""
}

if ($Help) {
    Show-Help
    exit 0
}

Write-Host "==================================================" -ForegroundColor Cyan
Write-Host "OpenGL Renderer - Build Script for Windows" -ForegroundColor Cyan
Write-Host "==================================================" -ForegroundColor Cyan

# Set build type
$BuildType = if ($Debug) { "Debug" } else { "Release" }
Write-Status "Build type: $BuildType"

# Clean build directory if requested
if ($Clean) {
    Write-Status "Cleaning build directory..."
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
    }
}

# Check if Conan is installed
Write-Status "Checking for Conan installation..."
try {
    $conanVersion = conan --version 2>$null
    if ($LASTEXITCODE -ne 0) {
        throw "Conan not found"
    }
    Write-Status "Found Conan: $conanVersion"
} catch {
    Write-Error "Conan is not installed or not in PATH!"
    Write-Host "Please install Conan first:"
    Write-Host "  pip install conan"
    Write-Host "  or download from: https://conan.io/downloads"
    exit 1
}

# Check if CMake is installed
Write-Status "Checking for CMake installation..."
try {
    $cmakeVersion = cmake --version 2>$null | Select-Object -First 1
    if ($LASTEXITCODE -ne 0) {
        throw "CMake not found"
    }
    Write-Status "Found CMake: $cmakeVersion"
} catch {
    Write-Error "CMake is not installed or not in PATH!"
    Write-Host "Please install CMake first:"
    Write-Host "  Download from: https://cmake.org/download/"
    Write-Host "  Or use winget: winget install Kitware.CMake"
    exit 1
}

# Create Conan profile if it doesn't exist
Write-Status "Setting up Conan profile..."
try {
    $profiles = conan profile list 2>$null
    if ($profiles -notcontains "default") {
        Write-Status "Creating default Conan profile..."
        conan profile detect --force
        if ($LASTEXITCODE -ne 0) {
            throw "Failed to create Conan profile"
        }
    } else {
        Write-Status "Default Conan profile already exists"
    }
} catch {
    Write-Error "Failed to setup Conan profile: $_"
    exit 1
}

# Install dependencies
Write-Status "Installing Conan dependencies..."
try {
    conan install . --output-folder=build --build=missing
    if ($LASTEXITCODE -ne 0) {
        throw "Conan install failed"
    }
} catch {
    Write-Error "Failed to install Conan dependencies: $_"
    exit 1
}

# Configure CMake
Write-Status "Configuring CMake..."
try {
    cmake -B build `
        -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake `
        -DCMAKE_BUILD_TYPE=$BuildType `
        -G "MinGW Makefiles"
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }
} catch {
    Write-Error "CMake configuration failed: $_"
    exit 1
}

# Build the project
Write-Status "Building the project..."
try {
    $cores = [Environment]::ProcessorCount
    cmake --build build --parallel $cores
    
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }
} catch {
    Write-Error "Build failed: $_"
    exit 1
}

# Success message
Write-Host ""
Write-Host "==================================================" -ForegroundColor Cyan
Write-Status "Build completed successfully!"
Write-Host "==================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "To run the application:" -ForegroundColor Yellow
Write-Host "  1. Open Command Prompt (cmd)" -ForegroundColor Gray
Write-Host "  2. Navigate to: cd /d `"$PWD`"" -ForegroundColor Gray
Write-Host "  3. Activate environment: build\conanrunenv-release-x86_64.bat" -ForegroundColor Gray
Write-Host "  4. Run executable: build\OpenGLRenderer.exe" -ForegroundColor Gray
Write-Host ""
Write-Host "OR using PowerShell with Conan:" -ForegroundColor Yellow
Write-Host "  cmd /c `"build\conanrunenv-release-x86_64.bat && build\OpenGLRenderer.exe`"" -ForegroundColor Gray
Write-Host ""
Write-Host "Build artifacts located in: .\build\" -ForegroundColor Gray