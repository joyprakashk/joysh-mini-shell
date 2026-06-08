# Windows Setup Guide for joysh

## ⚠️ Important Notice

**joysh uses POSIX system calls** (`fork()`, `pipe()`, `execvp()`, etc.) that are **NOT natively available on Windows**. These are Linux/Unix-specific APIs.

To build and run this project on Windows, you need a **Linux environment**. Below are your options.

---

## ✅ Option 1: WSL (Windows Subsystem for Linux) - RECOMMENDED

WSL gives you a real Linux environment inside Windows. This is the **best** option for development.

### Step-by-Step Setup

#### 1. Install WSL
```powershell
# Open PowerShell as Administrator
wsl --install
```

This installs:
- WSL 2
- Ubuntu (default distribution)
- All required components

#### 2. Restart Your Computer
After installation completes, restart Windows.

#### 3. Launch Ubuntu
- Open **"Ubuntu"** from the Start Menu
- First launch will take a few minutes to set up
- Create a Linux username and password when prompted

#### 4. Navigate to Your Project
```bash
# Your Windows E:\ drive is mounted at /mnt/e/
cd /mnt/e/mini-shell
```

#### 5. Install Build Tools
```bash
sudo apt update
sudo apt install build-essential gcc make git
```

#### 6. Build the Project
```bash
make clean
make
```

#### 7. Run the Shell
```bash
./bin/joysh
```

### WSL Tips

**Access Windows files from WSL:**
```bash
cd /mnt/c/Users/YourName/Documents  # C:\Users\YourName\Documents
cd /mnt/e/mini-shell                # E:\mini-shell
```

**Access WSL files from Windows:**
- Open File Explorer
- Type in address bar: `\\wsl$\Ubuntu\home\yourname\`

**Edit files in VS Code:**
```bash
# Inside WSL
code .
```

---

## ✅ Option 2: Docker Desktop - EASIEST

Docker runs a Linux container with all dependencies included.

### Step-by-Step Setup

#### 1. Install Docker Desktop
- Download: https://www.docker.com/products/docker-desktop/
- Install and restart Windows
- Launch Docker Desktop (must be running)

#### 2. Build the Docker Image
```powershell
# Open PowerShell or CMD
cd E:\mini-shell
docker build -t joysh .
```

This will:
- Create an Ubuntu container
- Install GCC and Make
- Build the joysh binary

#### 3. Run the Shell in Container
```powershell
docker run -it joysh
```

You'll be dropped into the joysh shell running inside the Linux container!

#### 4. Exit the Shell
```bash
exit
```

### Docker Commands Reference

```powershell
# Build image
docker build -t joysh .

# Run interactively
docker run -it joysh

# Run with mounted directory (to access Windows files)
docker run -it -v E:\mini-shell:/workspace joysh

# List images
docker images

# Remove image
docker rmi joysh
```

---

## ⚠️ Option 3: MinGW/Cygwin - LIMITED

**NOT RECOMMENDED** - Windows doesn't properly support `fork()` and other POSIX APIs.

MinGW and Cygwin provide some POSIX compatibility, but:
- `fork()` doesn't work reliably on Windows
- Process management is fundamentally different
- Pipes and file descriptors behave differently
- You'll encounter many issues

If you still want to try:

### MinGW Setup
1. Install MinGW-w64: https://www.mingw-w64.org/
2. Add to PATH: `C:\mingw64\bin`
3. Try compiling (may fail): `gcc src/*.c -Iinclude -o joysh.exe`

**Expected Issues:**
- `fork()` not available → compilation errors
- `pipe()` limited support
- Signal handling different

---

## ✅ Option 4: Linux Virtual Machine

Run Ubuntu in a virtual machine.

### Setup

1. **Install VirtualBox or VMware:**
   - VirtualBox: https://www.virtualbox.org/
   - VMware Workstation Player (free): https://www.vmware.com/

2. **Download Ubuntu ISO:**
   - https://ubuntu.com/download/desktop
   - Ubuntu 22.04 LTS recommended

3. **Create VM:**
   - 2 GB RAM minimum (4 GB recommended)
   - 20 GB disk space
   - Install Ubuntu

4. **Transfer Project:**
   - Use shared folders
   - Or: `git clone` if you pushed to GitHub
   - Or: Copy files manually

5. **Build in VM:**
   ```bash
   cd ~/mini-shell
   make
   ./bin/joysh
   ```

---

## 🎯 Recommended Path for Windows Users

### For Development & Interviews:
**Use WSL** - It's fast, integrated with Windows, and gives you a real Linux environment.

### For Quick Testing:
**Use Docker** - One command to run, no setup needed.

### For Learning Linux:
**Use a VM** - Full Linux experience, learn system administration.

---

## 📋 Quick Command Reference

### WSL
```bash
# Access project
cd /mnt/e/mini-shell

# Build
make

# Run
./bin/joysh

# Edit in VS Code
code .
```

### Docker
```powershell
# Build image
docker build -t joysh .

# Run shell
docker run -it joysh

# Run with code access
docker run -it -v E:\mini-shell:/workspace joysh
```

---

## 🐛 Troubleshooting

### "wsl --install" doesn't work
**Solution:**
1. Enable virtualization in BIOS
2. Enable Windows features:
   ```powershell
   dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
   dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
   ```
3. Restart and try again

### Docker build fails
**Solution:**
1. Ensure Docker Desktop is running
2. Check virtualization is enabled
3. Try: `docker system prune` then rebuild

### "Permission denied" in WSL
**Solution:**
```bash
chmod +x bin/joysh
./bin/joysh
```

### Can't access Windows files from WSL
**Solution:**
Windows drives are mounted at `/mnt/`:
- `C:\` → `/mnt/c/`
- `E:\` → `/mnt/e/`

---

## 🚀 Getting Started (Recommended Path)

### Step 1: Install WSL
```powershell
# PowerShell (Admin)
wsl --install
```

### Step 2: Restart
Restart your computer.

### Step 3: Open Ubuntu
Find "Ubuntu" in Start Menu and launch.

### Step 4: Setup
```bash
# Update package lists
sudo apt update

# Install build tools
sudo apt install build-essential

# Navigate to project
cd /mnt/e/mini-shell

# Build
make

# Run
./bin/joysh
```

### Step 5: Test
```bash
joysh$ pwd
joysh$ ls -la
joysh$ echo "hello" | cat
joysh$ stats
joysh$ exit
```

---

## 📚 Additional Resources

### WSL Documentation
- https://docs.microsoft.com/en-us/windows/wsl/

### Docker Documentation
- https://docs.docker.com/desktop/windows/

### Linux Basics
- https://ubuntu.com/tutorials/command-line-for-beginners

---

## ❓ FAQ

**Q: Why doesn't this work on Windows natively?**  
A: Windows uses different APIs (`CreateProcess`, `DuplicateHandle`, etc.) instead of POSIX calls (`fork`, `pipe`, etc.). This is a Linux/Unix shell demonstrating Linux system programming.

**Q: Can I port this to Windows?**  
A: Yes, but you'd need to rewrite all process management using Windows APIs. That would be a different project demonstrating Windows programming, not Linux systems programming.

**Q: Which option is fastest?**  
A: WSL is fastest for development. Docker is easiest for one-time testing.

**Q: Will this affect my interview preparation?**  
A: No! Interviewers understand that Linux projects run on Linux. Using WSL/Docker shows you know how to work with different environments - that's a plus!

**Q: Can I develop in Windows and test in WSL?**  
A: Yes! Edit files in Windows (VS Code, Notepad++, etc.) and build/run in WSL. Files sync automatically.

---

## ✅ Verification Checklist

After setup:
- [ ] Can navigate to project: `cd /mnt/e/mini-shell`
- [ ] Can build: `make` succeeds
- [ ] Binary created: `ls bin/joysh`
- [ ] Can run: `./bin/joysh` starts shell
- [ ] Basic commands work: `pwd`, `ls`, `echo`
- [ ] Can exit: `exit` command works

---

## 🎓 For Interviews

When discussing this project in interviews:

✅ **DO SAY:**
- "I developed this on WSL/Docker to use Linux system calls"
- "I understand Windows has different APIs, but I wanted to demonstrate Linux systems programming"
- "I'm comfortable with both Windows and Linux development environments"

❌ **DON'T SAY:**
- "I couldn't get it to work on Windows"
- "I only know Windows development"

**Interviewers will RESPECT** that you:
1. Chose the right tool (Linux) for the job (systems programming)
2. Set up a proper development environment
3. Understand platform differences

---

## 🚀 Ready to Start?

**Run this now:**

```powershell
# PowerShell (Admin)
wsl --install
```

Then restart, open Ubuntu, and you're ready to build professional Linux systems projects!

---

**Questions?** See the main [README.md](README.md) for project documentation.
