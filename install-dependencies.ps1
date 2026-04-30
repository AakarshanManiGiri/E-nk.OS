# E-nk.OS Dependencies Installation Script (Windows PowerShell)
# This script installs all required dependencies for the project

Write-Host "================================" -ForegroundColor Cyan
Write-Host "E-nk.OS - Dependency Installer" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Check if Python is installed (required for PlatformIO)
Write-Host "Checking for Python..." -ForegroundColor Yellow
$pythonCheck = python --version 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "Python not found! Please install Python 3.6+ from https://www.python.org/" -ForegroundColor Red
    exit 1
}
Write-Host "✓ Python found: $pythonCheck" -ForegroundColor Green
Write-Host ""

# Install/Update PlatformIO
Write-Host "Installing/Updating PlatformIO CLI..." -ForegroundColor Yellow
pip install -U platformio
if ($LASTEXITCODE -ne 0) {
    Write-Host "Failed to install PlatformIO" -ForegroundColor Red
    exit 1
}
Write-Host "✓ PlatformIO CLI installed successfully" -ForegroundColor Green
Write-Host ""

# Install PlatformIO IDE extensions (if VSCode is available)
Write-Host "PlatformIO setup complete!" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. Open this project folder in VS Code"
Write-Host "2. Install the 'PlatformIO IDE' extension from the VS Code marketplace"
Write-Host "3. PlatformIO will automatically handle library dependencies"
Write-Host ""
Write-Host "To build the project, run:" -ForegroundColor Cyan
Write-Host "  platformio run" -ForegroundColor White
Write-Host ""
Write-Host "To upload to device, run:" -ForegroundColor Cyan
Write-Host "  platformio run -t upload" -ForegroundColor White
Write-Host ""
