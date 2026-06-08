# joysh

A UNIX shell implementation in C demonstrating process management, inter-process communication, and I/O redirection.

[![CI](https://github.com/yourusername/joysh/workflows/CI%20Build%20&%20Test/badge.svg)](https://github.com/yourusername/joysh/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## Features

- Command execution via `fork(2)` and `execvp(3)`
- Pipeline support with `pipe(2)`
- I/O redirection (`<`, `>`, `>>`, `2>`)
- Background job execution (`&`)
- Signal handling (SIGINT, SIGTSTP)
- Built-in commands: `cd`, `pwd`, `exit`, `clear`, `history`, `stats`
- Command logging and metrics tracking

## Building

### Prerequisites

- Linux (Ubuntu 20.04+) or WSL2
- GCC 7.5+
- GNU Make 4.1+

### Compile

```bash
make
```

Binary output: `bin/joysh`

### Docker

```bash
docker build -t joysh .
docker run -it joysh
```

## Usage

```bash
$ ./bin/joysh
joysh$ ls -la | grep txt > files.txt
joysh$ cat < input.txt | sort
joysh$ sleep 10 &
[Background] PID: 1234
joysh$ stats
=== joysh Statistics ===
Total commands executed: 3
Background jobs: 1
Pipes executed: 1
Failed commands: 0
Shell uptime: 45 seconds
joysh$ exit
```

## Architecture

```
Input → Tokenizer → Parser → Executor
                              ↓
                     fork/exec/pipe/wait
```

The shell is implemented as modular components:

| Module | Purpose | Key System Calls |
|--------|---------|------------------|
| tokenizer | Input parsing | - |
| parser | Command structure building | - |
| executor | Process management | fork, execvp, waitpid, pipe, dup2 |
| builtins | Shell-internal commands | chdir, getcwd |
| redirect | I/O redirection | open, close, dup2 |
| signals | Signal handling | sigaction |
| logger | Command logging | fopen, fprintf |

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for detailed design.

## Implementation Details

### Process Management

Commands execute in child processes created via `fork(2)`. The parent waits on foreground jobs with `waitpid(2)` to prevent zombies. Background jobs (`&`) run asynchronously.

### Pipes

Pipelines use `pipe(2)` to create kernel buffers (64KB on Linux). File descriptors are duplicated with `dup2(2)` to connect process stdin/stdout. All unused pipe ends are closed to prevent deadlocks.

### Redirection

File redirection uses `open(2)` with appropriate flags:
- Input (`<`): `O_RDONLY`
- Output (`>`): `O_WRONLY | O_CREAT | O_TRUNC`
- Append (`>>`): `O_WRONLY | O_CREAT | O_APPEND`

File descriptors are duplicated to stdin/stdout via `dup2(2)`.

### Signal Handling

The parent shell catches SIGINT and SIGTSTP using `sigaction(2)` with `SA_RESTART` to prevent interruption of system calls. Child processes inherit default signal handlers.

## Testing

```bash
# Run test suite
chmod +x tests/test.sh
./tests/test.sh

# Memory leak check (requires Valgrind)
make valgrind
```

## System Requirements

- POSIX-compliant OS (Linux, *BSD, macOS)
- Required system calls: fork, execvp, waitpid, pipe, dup2, open, close, chdir, sigaction

**Windows users:** See [WINDOWS_SETUP.md](WINDOWS_SETUP.md) for WSL/Docker instructions.

## Performance

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Tokenization | O(n) | Single pass |
| Parsing | O(n) | Single pass |
| Command execution | O(1) | Fork overhead ~100-200μs |
| Pipe creation | O(k) | k = number of pipes |

Memory footprint: ~50KB base + ~1KB per command.

## Limitations

- No job control (`fg`, `bg`, `jobs`)
- No environment variable expansion
- No command substitution
- No globbing
- No shell scripting constructs
- No command-line editing (readline)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## Documentation

- [Architecture Design](docs/ARCHITECTURE.md)
- [Build Instructions](docs/BUILD.md)
- [System Call Reference](docs/SYSCALLS.md)

## License

MIT - see [LICENSE](LICENSE)

## References

- Stevens & Rago, *Advanced Programming in the UNIX Environment* (3rd ed.)
- Kerrisk, *The Linux Programming Interface*
- `man 2 fork`, `man 2 pipe`, `man 2 execve`
