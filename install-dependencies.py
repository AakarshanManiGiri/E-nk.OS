#!/usr/bin/env python3
"""
E-nk.OS Dependencies Installation Script
Platform-independent installer for all project dependencies
"""

import sys
import subprocess
import platform
import os

class Colors:
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    RESET = '\033[0m'

def print_header(text):
    print(f"{Colors.CYAN}{'='*40}{Colors.RESET}")
    print(f"{Colors.CYAN}{text}{Colors.RESET}")
    print(f"{Colors.CYAN}{'='*40}{Colors.RESET}")
    print()

def print_success(text):
    print(f"{Colors.GREEN}✓ {text}{Colors.RESET}")

def print_info(text):
    print(f"{Colors.YELLOW}→ {text}{Colors.RESET}")

def print_error(text):
    print(f"{Colors.RED}✗ {text}{Colors.RESET}")

def run_command(cmd, description):
    """Execute a command and handle errors"""
    print_info(description)
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        if result.returncode != 0:
            print_error(f"Failed: {result.stderr}")
            return False
        return True
    except Exception as e:
        print_error(f"Error: {str(e)}")
        return False

def check_python():
    """Verify Python version"""
    print_info("Checking Python version...")
    version = f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"
    if sys.version_info < (3, 6):
        print_error(f"Python 3.6+ required, found {version}")
        return False
    print_success(f"Python {version} found")
    return True

def check_pip():
    """Verify pip is available"""
    print_info("Checking pip...")
    try:
        subprocess.run([sys.executable, "-m", "pip", "--version"], capture_output=True, check=True)
        print_success("pip found")
        return True
    except subprocess.CalledProcessError:
        print_error("pip not found")
        return False

def install_platformio():
    """Install/upgrade PlatformIO"""
    print_info("Installing/Updating PlatformIO CLI...")
    cmd = f"{sys.executable} -m pip install -U platformio"
    if run_command(cmd, "Running pip install..."):
        print_success("PlatformIO installed successfully")
        return True
    return False

def get_os_type():
    """Detect operating system"""
    system = platform.system()
    if system == "Windows":
        return "windows"
    elif system == "Darwin":
        return "macos"
    else:
        return "linux"

def main():
    print_header("E-nk.OS - Dependency Installer")
    
    # Step 1: Check Python
    if not check_python():
        print_error("Please install Python 3.6+")
        sys.exit(1)
    print()
    
    # Step 2: Check pip
    if not check_pip():
        print_error("Please ensure pip is installed")
        sys.exit(1)
    print()
    
    # Step 3: Install PlatformIO
    if not install_platformio():
        print_error("Failed to install PlatformIO")
        sys.exit(1)
    print()
    
    # Step 4: Success message
    print_success("All dependencies installed successfully!")
    print()
    print(f"{Colors.CYAN}Next Steps:{Colors.RESET}")
    print("1. Open this project folder in VS Code")
    print("2. Install the 'PlatformIO IDE' extension from the VS Code marketplace")
    print("3. PlatformIO will automatically resolve library dependencies")
    print()
    print(f"{Colors.CYAN}Building the project:{Colors.RESET}")
    print(f"  {Colors.YELLOW}platformio run{Colors.RESET}")
    print()
    print(f"{Colors.CYAN}Uploading to device:{Colors.RESET}")
    print(f"  {Colors.YELLOW}platformio run -t upload{Colors.RESET}")
    print()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nInstallation cancelled by user")
        sys.exit(1)
