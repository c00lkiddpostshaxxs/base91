#!/bin/bash

# --- 1. Detect OS and Install Compiler ---
clear
if ! command -v gcc &> /dev/null; then
    echo "Compiler not found. Installing..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        [ -f /etc/os-release ] && . /etc/os-release && case "$ID" in
            ubuntu|debian) sudo apt update && sudo apt install -y gcc ;;
            fedora|rhel|rocky|almalinux) sudo dnf install -y gcc ;;
            arch) sudo pacman -S --noconfirm gcc ;;
            *) echo "Unsupported OS"; exit 1 ;;
        esac
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        if ! command -v brew &> /dev/null; then
            echo "Installing Homebrew..."
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            eval "$(/opt/homebrew/bin/brew shellenv)"
        fi
        echo "Installing GCC..."
        brew install gcc
    fi
fi

# --- 2. Download and Compile ---
echo "Downloading basE91..."
curl -fsSL 'https://github.com/c00lkiddpostshaxxs/base91/releases/download/v1/base91.c' -o base91.c
echo "Compiling..."
gcc -O2 base91.c -o base91
rm base91.c

# --- 3. Optional Install ---
echo "--------------------------------------------------------"
echo "Installation complete."
echo -n "Move to /usr/local/bin/ system-wide? (Requires password) [y/n]: "
read -r response

if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    echo "--- Please enter your password when prompted ---"
    sudo mv base91 /usr/local/bin/
    CMD="base91"
else
    chmod +x base91
    mv base91 "$HOME/base91"
    CMD="$HOME/base91"
fi

# --- 4. Test ---
echo "--------------------------------------------------------"
echo "Installation successful! Running test..."
echo 'Hello, World!' | "$CMD"

# --- 5. Cleanup ---
rm -- "$0"
