# Architecture

## Overview

joysh is a modular UNIX shell implementing process management, IPC, and I/O redirection using POSIX system calls.

## Components

### Tokenizer (`tokenizer.c`)

Breaks raw input into tokens. Handles quotes, special characters (`|`, `<`, `>`, `&`), and whitespace.

**Interface:**
```c
char **tokenize(const char *input, int *token_count);
void free_tokens(char **tokens, int token_count);
```

### Parser (`parser.c`)

Converts tokens into command structures. Detects pipes, redirections, and background execution.

**Interface:**
```c
Command *parse_command(char **tokens, int token_count, int *cmd_count);
void free_commands(Command *commands, int cmd_count);
int is_builtin(const char *cmd);
```

**Command Structure:**
```c
typedef struct {
    char **args;
    char *input_file;
    char *output_file;
    int append;
    int background;
    int stderr_redirect;
} Command;
```

### Executor (`executor.c`)

Handles process creation and IPC. Core system call logic.

**Interface:**
```c
int execute_command(Command *cmd);
int execute_pipeline(Command *commands, int cmd_count);
```

**Process Flow:**
1. Fork child process
2. Setup redirections in child
3. Execute command via execvp
4. Parent waits (or continues for background jobs)

**Pipeline Flow:**
1. Create N-1 pipes for N commands
2. Fork N children
3. Connect pipes via dup2 in each child
4. Close all pipe FDs in parent
5. Wait for all children

### Builtins (`builtins.c`)

Shell-internal commands that cannot be external programs.

**Commands:**
- `cd` - uses `chdir(2)`
- `pwd` - uses `getcwd(3)`
- `exit` - terminates shell
- `clear` - sends escape codes
- `history` - displays command history
- `stats` - shows shell metrics

### Redirection (`redirect.c`)

File I/O redirection via `open(2)` and `dup2(2)`.

**Interface:**
```c
int setup_input_redirect(const char *filename);
int setup_output_redirect(const char *filename, int append);
int setup_stderr_redirect(const char *filename);
```

### Signals (`signals.c`)

Signal handling using `sigaction(2)`.

**Strategy:**
- Parent ignores SIGINT/SIGTSTP (shell continues)
- Children inherit default handlers (terminate/stop)
- Use `SA_RESTART` flag to prevent syscall interruption

### Logger (`logger.c`)

Command logging to `~/.joysh_logs` with timestamps and status tracking.

## Execution Flow

```
User Input
    ↓
Tokenizer → ["ls", "|", "grep", "txt"]
    ↓
Parser → [{cmd: "ls"}, {cmd: "grep", args: ["txt"]}]
    ↓
Executor
    ↓
  pipe()
    ↓
fork() → Child 1 (ls)      fork() → Child 2 (grep)
         dup2(pipe[1], 1)           dup2(pipe[0], 0)
         execvp("ls")               execvp("grep")
    ↓
waitpid() × 2
```

## File Descriptor Management

Standard descriptors:
- 0: stdin
- 1: stdout
- 2: stderr

**Pipe Example:**
```c
pipe(pipefd);  // pipefd[0] = read, pipefd[1] = write

// Child 1 (writer)
close(pipefd[0]);
dup2(pipefd[1], STDOUT_FILENO);
close(pipefd[1]);

// Child 2 (reader)
close(pipefd[1]);
dup2(pipefd[0], STDIN_FILENO);
close(pipefd[0]);

// Parent
close(pipefd[0]);
close(pipefd[1]);
```

## Memory Management

- All heap allocations freed before exit
- Token arrays freed after parsing
- Command structures freed after execution
- History bounded to MAX_HISTORY entries

## Concurrency Model

- Single-threaded with fork-based process creation
- No shared state between processes
- Parent-child synchronization via waitpid

## Error Handling

- System call errors checked and reported via `perror(3)`
- Shell continues on non-fatal errors
- Graceful shutdown on EOF (Ctrl+D)

## System Calls

| Call | Purpose | Location |
|------|---------|----------|
| fork | Create process | executor.c |
| execvp | Execute program | executor.c |
| waitpid | Wait for child | executor.c |
| pipe | Create IPC channel | executor.c |
| dup2 | Duplicate FD | executor.c, redirect.c |
| open | Open file | redirect.c |
| close | Close FD | redirect.c, executor.c |
| chdir | Change directory | builtins.c |
| getcwd | Get directory | builtins.c |
| sigaction | Register handler | signals.c |

## Build System

Makefile targets:
- `make` - default build
- `make clean` - remove artifacts
- `make debug` - build with symbols
- `make release` - optimized build
- `make valgrind` - memory leak check

## Testing

Test suite (`tests/test.sh`) verifies:
- Basic command execution
- Pipe functionality
- Redirection
- Background jobs
- Built-in commands
- Error handling

## Performance Characteristics

- Fork overhead: ~100-200μs per command
- Pipe buffer: 64KB (kernel-managed)
- Memory: ~50KB base + ~1KB per command
- No memory leaks (Valgrind verified)

## Limitations

Current implementation does not support:
- Job control (fg/bg/jobs)
- Environment variable expansion ($VAR)
- Command substitution $(cmd)
- Globbing (*.txt)
- Shell scripting (if/while/for)
- Command-line editing (readline)

These could be added as future enhancements.
