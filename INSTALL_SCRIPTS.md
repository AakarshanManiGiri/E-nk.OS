# Installation Scripts Reference

## Available Installation Methods

Choose the script that matches your operating system:

### 1. **install-dependencies.ps1** (Windows PowerShell)
- **Platform**: Windows
- **Usage**: `.\install-dependencies.ps1`
- **What it does**:
  - ✓ Checks for Python installation
  - ✓ Installs/updates PlatformIO CLI via pip
  - ✓ Provides setup instructions

### 2. **install-dependencies.sh** (Bash/Shell)
- **Platform**: Linux, macOS, WSL
- **Usage**: `chmod +x install-dependencies.sh && ./install-dependencies.sh`
- **What it does**:
  - ✓ Checks for Python 3 installation
  - ✓ Installs/updates PlatformIO CLI via pip3
  - ✓ Initializes PlatformIO project
  - ✓ Provides setup instructions

### 3. **install-dependencies.py** (Python - Cross-Platform)
- **Platform**: Windows, Linux, macOS, WSL
- **Usage**: `python install-dependencies.py` or `python3 install-dependencies.py`
- **What it does**:
  - ✓ Detects operating system
  - ✓ Checks Python version (3.6+)
  - ✓ Verifies pip availability
  - ✓ Installs/updates PlatformIO CLI
  - ✓ Shows colored status messages
  - ✓ Provides comprehensive setup instructions

### 4. **platformio.ini** (Project Configuration)
- **Type**: PlatformIO configuration file
- **Purpose**: Defines:
  - Board: ESP32-S3-DevKitC-1
  - Framework: Arduino
  - All library dependencies
  - Build flags and settings
  - Upload/monitor settings

## Quick Decision Tree

```
Are you on Windows?
├─ Yes → Use install-dependencies.ps1 or install-dependencies.py
└─ No → Use install-dependencies.sh or install-dependencies.py

Do you prefer Python scripts?
├─ Yes → Use install-dependencies.py (works everywhere)
└─ No → Use native script for your OS
```

## What Gets Installed

All scripts install the same things:

1. **PlatformIO CLI** - Build system and package manager
2. **ESP32-S3 Platform** - Board support (via PlatformIO)
3. **Arduino Framework** - Core framework (via PlatformIO)

All other dependencies are defined in `platformio.ini`:
- Adafruit GFX Library
- Adafruit EPD Library
- SdFat Library
- OpenBook_EPD Library
- BabelTypesetterGFX Library

## System Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| Python | 3.6 | 3.9+ |
| pip | Latest | Latest |
| Disk Space | 500 MB | 1 GB+ |
| Memory | 2 GB | 4 GB+ |

## Troubleshooting

### "Permission Denied" (Linux/macOS)
```bash
chmod +x install-dependencies.sh
./install-dependencies.sh
```

### "Execution Policy" (Windows PowerShell)
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
.\install-dependencies.ps1
```

### "Python not found"
- Install Python 3.6+ from https://www.python.org/
- Make sure to check "Add Python to PATH" during installation

### Script exits with error
- Check that Python is properly installed: `python --version`
- Ensure pip works: `python -m pip --version`
- Try the Python script: `python install-dependencies.py`

## After Installation

1. Open the project in VS Code
2. Install "PlatformIO IDE" extension
3. Build: `platformio run`
4. Upload: `platformio run -t upload`

For detailed instructions, see [SETUP.md](SETUP.md)
