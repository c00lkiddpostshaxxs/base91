# basE91
### This is a basE91 encoder and decoder! Install it by following the instructions below!
### Auto install:
```bash
curl -fsSL https://github.com/c00lkiddpostshaxxs/base91/releases/download/v1/install.sh -o install.sh && bash install.sh
```
### Manual install:
**For Linux:**   
Step 1: Check if gcc is installed already:
```bash
which gcc
```
If you get a path *(e.g., /usr/bin/gcc)*, it is installed. *If you get an **error or no output**, you need to install it with the line below:*   
```bash
[ -f /etc/os-release ] && . /etc/os-release && case "$ID" in ubuntu|debian) sudo apt update && sudo apt install -y gcc ;; fedora|rhel|rocky|almalinux) sudo dnf install -y gcc ;; arch) sudo pacman -S --noconfirm gcc ;; opensuse*|sles) sudo zypper install -y gcc ;; *) echo "Unsupported Official OS" ;; esac
```
Step 2: Install the C file:
```bash
curl -fsSL 'https://github.com/c00lkiddpostshaxxs/base91/releases/download/v1/base91.c' -o base91.c
```
Step 3: Compile it:
```bash
gcc -O2 base91.c -o base91
```
Step 4 ***(Optional)***: Move it to your local binaries folder:
```bash
sudo mv base91 /usr/local/bin/
```
Step 5: Try it!
```bash
echo 'Hello, World!' | base91
```
*If fail, try:*
```bash
echo 'Hello, World!' | ~/Downloads/base91
```
**For macOS:**
Step 1: Check if gcc is installed:
```bash
gcc --version
```
*If it says **"Apple clang version..."**, you do not have the real GCC. You are using Apple's default Clang compiler. Run the command below to compile it:*   
```bash
clang -O2 base91.c -o base91
```
*If it pops up a window asking you to install **"command line developer tools",** it means you have no compiler installed at all. Install it with the instructions below:*   
Check if Homebrew is installed on your Mac:
```bash
which brew
```
*If it prints a folder path, Homebrew is installed! If **NOT** it will appear "command not found" or an error.*   
Copy this command into your terminal to install Homebrew if not:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
It'll ask for your password. Type your password, it won't show then hit Enter.   
Once done, type this command:
```bash
brew install gcc
```
Step 2: Install the C file:
```bash
curl -fsSL 'https://github.com/c00lkiddpostshaxxs/base91/releases/download/v1/base91.c' -o base91.c
```
Step 3: Compile it:
```bash
gcc -O2 base91.c -o base91
```
Step 4 ***(Optional)***: Move it to your local binaries folder:
```bash
sudo mv base91 /usr/local/bin/
```
Step 5: Try it!
```bash
echo 'Hello, World!' | base91
```
*If fail, try:*
```bash
echo 'Hello, World!' | ~/Downloads/base91
```
