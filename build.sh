#!/bin/bash

# OpenGL Renderer - Build Script for Linux/macOS
# This script installs Conan dependencies and builds the project

set -e  # Exit on any error

echo "=================================================="
echo "OpenGL Renderer - Build Script"
echo "=================================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if Conan is installed
print_status "Checking for Conan installation..."
if ! command -v conan &> /dev/null; then
    print_error "Conan is not installed!"
    echo "Please install Conan first:"
    echo "  pip install conan"
    echo "  or"
    echo "  pip3 install conan"
    exit 1
fi

# Check if CMake is installed
print_status "Checking for CMake installation..."
if ! command -v cmake &> /dev/null; then
    print_error "CMake is not installed!"
    echo "Please install CMake first:"
    echo "  sudo apt-get install cmake  # Ubuntu/Debian"
    echo "  brew install cmake          # macOS"
    exit 1
fi

# Check Conan version
CONAN_VERSION=$(conan --version 2>&1 | grep -o '[0-9]\+\.[0-9]\+' | head -1)
print_status "Found Conan version: $CONAN_VERSION"

# Create Conan profile if it doesn't exist
print_status "Setting up Conan profile..."
if ! conan profile list | grep -q "default"; then
    print_status "Creating default Conan profile..."
    conan profile detect --force
else
    print_status "Default Conan profile already exists"
fi

# Install dependencies
print_status "Installing Conan dependencies..."
conan install . --output-folder=build --build=missing

if [ $? -ne 0 ]; then
    print_error "Failed to install Conan dependencies"
    exit 1
fi

# Configure CMake
print_status "Configuring CMake..."
cmake -B build \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed"
    exit 1
fi

# Build the project
print_status "Building the project..."
cmake --build build --parallel $(nproc 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    print_error "Build failed"
    exit 1
fi

# Success message
echo ""
echo "=================================================="
print_status "Build completed successfully!"
echo "=================================================="
echo ""
echo "To run the application:"
echo "  ./build/OpenGLRenderer"
echo ""
echo "To run with debug output:"
echo "  MESA_DEBUG=silent ./build/OpenGLRenderer"
echo ""