# E-nk.OS - Dependency Installation Guide

This guide will help you set up all dependencies needed to build and run the E-nk.OS project.

## Prerequisites

Before running the installation scripts, ensure you have:
- **Python 3.6+** installed ([Download](https://www.python.org/downloads/))
- **Git** installed (for cloning library repositories)
- **VS Code** (recommended) for development

## Quick Start

### Option 1: PowerShell (Windows)

```powershell
# Run the PowerShell installation script
.\install-dependencies.ps1
```

If you get an execution policy error, run:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Option 2: Bash/Shell (Linux/macOS)

```bash
# Make the script executable
chmod +x install-dependencies.sh

# Run the installation script
./install-dependencies.sh
```

### Option 3: Python (Cross-Platform)

```bash
# Run the Python installation script
python install-dependencies.py
# or
python3 install-dependencies.py
```

## Project Dependencies

This project requires the following:

### Framework & Board Support
- **Arduino Framework** - Installed via PlatformIO
- **ESP32-S3 Board Support** - PlatformIO handles this

### Libraries
| Library | Purpose | Source |
|---------|---------|--------|
| **Adafruit GFX** | Graphics rendering library | PlatformIO Registry |
| **Adafruit EPD** | E-Ink display driver | PlatformIO Registry |
| **SdFat** | SD card file system | PlatformIO Registry |
| **OpenBook_EPD** | E-Ink display support | GitHub (joeycastillo) |
| **BabelTypesetterGFX** | Text rendering | GitHub (joeycastillo) |

### Build Tools
- **PlatformIO** - Build system and package manager
- **Python 3.6+** - Required by PlatformIO

## After Installation

### 1. Install VS Code Extensions

- **PlatformIO IDE** - Official extension for embedded development
  - Search for "PlatformIO" in VS Code extensions marketplace
  - Install the official extension by PlatformIO

### 2. Verify Installation

Once PlatformIO is installed and the IDE extension is active, the dependencies will be automatically resolved:

```bash
# Build the project
platformio run

# List all dependencies
platformio lib list

# Check board configuration
platformio boards esp32-s3-devkitc-1
```

### 3. Build & Upload

```bash
# Build for ESP32-S3
platformio run

# Build and upload to connected device
platformio run -t upload

# Monitor serial output
platformio device monitor
```

## Troubleshooting

### "platformio: command not found"
```bash
# Ensure PlatformIO is in PATH
python -m platformio --version

# Or use full path
python -m platformio run
```

### Library download fails
```bash
# Force library update
platformio lib update

# Clean build
platformio run --target clean
platformio run
```

### Board not detected
```bash
# List connected devices
platformio device list

# Check USB permissions (Linux)
sudo usermod -a -G dialout $USER
```

## Manual Installation (Advanced)

If the scripts don't work, you can manually install:

```bash
# Install Python dependencies
pip install platformio esptool

# Build the project
platformio run

# Upload to device
platformio run -t upload
```

## Project Configuration

The `platformio.ini` file contains:
- **Board**: ESP32-S3-DevKitC-1
- **Framework**: Arduino
- **Display**: 300x400 E-Ink display
- **Rotation**: 0 degrees (portrait)

Modify `platformio.ini` to change board settings or add additional configurations.

## Need Help?

- **PlatformIO Docs**: https://docs.platformio.org/
- **ESP32 Docs**: https://docs.espressif.com/
- **Adafruit Libraries**: https://github.com/adafruit
- **OpenBook Project**: https://github.com/joeycastillo/OpenBook_EPD

---

**Note**: The first build may take longer as it downloads and compiles all dependencies.
