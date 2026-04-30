#!/bin/bash
# E-nk.OS Dependencies Installation Script (Linux/macOS)
# This script installs all required dependencies for the project

echo "================================"
echo "E-nk.OS - Dependency Installer"
echo "================================"
echo ""

# Check if Python is installed (required for PlatformIO)
echo "Checking for Python..."
if ! command -v python3 &> /dev/null; then
    echo "Python 3 not found! Please install Python 3.6+ from https://www.python.org/"
    exit 1
fi
PYTHON_VERSION=$(python3 --version)
echo "✓ Python found: $PYTHON_VERSION"
echo ""

# Install/Update PlatformIO
echo "Installing/Updating PlatformIO CLI..."
pip3 install -U platformio
if [ $? -ne 0 ]; then
    echo "Failed to install PlatformIO"
    exit 1
fi
echo "✓ PlatformIO CLI installed successfully"
echo ""

# Initialize PlatformIO
echo "Initializing PlatformIO project..."
platformio project init --board esp32-s3-devkitc-1 --ide vscode
echo ""

echo "✓ PlatformIO setup complete!"
echo ""
echo "Next steps:"
echo "1. Open this project folder in VS Code"
echo "2. Install the 'PlatformIO IDE' extension from the VS Code marketplace"
echo "3. PlatformIO will automatically handle library dependencies"
echo ""
echo "To build the project, run:"
echo "  platformio run"
echo ""
echo "To upload to device, run:"
echo "  platformio run -t upload"
echo ""
