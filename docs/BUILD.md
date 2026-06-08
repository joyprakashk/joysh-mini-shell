# Build Instructions

## Prerequisites

- Linux (Ubuntu 20.04+, Debian 10+, Fedora 33+) or WSL2
- GCC 7.5+ or Clang 10+
- GNU Make 4.1+
- (Optional) Docker, Valgrind

## Quick Start

```bash
make
./bin/joysh
```

## Build Targets

```bash
make           # Default build
make clean     # Remove artifacts
make debug     # Build with debug symbols (-g -O0)
make release   # Optimized build (-O2)
make valgrind  # Run with memory leak detection
```

## Docker

```bash
docker build -t joysh .
docker run -it joysh
```

## Installation

```bash
sudo make install   # Installs to /usr/local/bin
sudo make uninstall # Removes from /usr/local/bin
```

## Testing

```bash
chmod +x tests/test.sh
./tests/test.sh
```

## Troubleshooting

### Compilation Errors

**Problem:** `make: command not found`  
**Solution:** Install build tools
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# Fedora/RHEL
sudo dnf install gcc make

# Arch
sudo pacman -S base-devel
```

**Problem:** Missing headers  
**Solution:** Ensure all files in `src/` and `include/` are present

### Runtime Errors

**Problem:** Segmentation fault  
**Solution:** Run with debugger
```bash
make debug
gdb ./bin/joysh
(gdb) run
(gdb) backtrace
```

**Problem:** Command not found  
**Solution:** Ensure command exists in PATH
```bash
which <command>
```

### Memory Leaks

Check with Valgrind:
```bash
make valgrind
# Or manually:
valgrind --leak-check=full ./bin/joysh
```

## Windows Setup

This project uses POSIX system calls not available on Windows. Options:

### WSL2 (Recommended)

```powershell
wsl --install
```

Restart, then open Ubuntu:
```bash
cd /mnt/e/mini-shell
sudo apt update
sudo apt install build-essential
make
./bin/joysh
```

### Docker

Install Docker Desktop, then:
```powershell
docker build -t joysh .
docker run -it joysh
```

See [WINDOWS_SETUP.md](../WINDOWS_SETUP.md) for details.

## Build Configuration

Edit `Makefile` to customize:

```makefile
CC = gcc              # Or clang
CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude
LDFLAGS =             # Add libraries if needed
```

## Verification

After building:

```bash
# Check binary exists
ls -lh bin/joysh

# Check file type
file bin/joysh

# Run basic commands
echo "pwd" | ./bin/joysh
echo "echo test" | ./bin/joysh
```

Expected output:
```
bin/joysh: ELF 64-bit LSB executable
```

## Clean Build

If experiencing issues:

```bash
make clean
make
```

This removes all build artifacts and recompiles from scratch.
