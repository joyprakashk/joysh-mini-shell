# System Call Reference

Core POSIX system calls used in joysh implementation.

## Process Management

### fork(2)

Create child process by duplicating caller.

```c
#include <unistd.h>
pid_t fork(void);
```

**Returns:**
- 0 in child process
- Child PID in parent
- -1 on error

**Usage:**
```c
pid_t pid = fork();
if (pid < 0) {
    perror("fork");
} else if (pid == 0) {
    // Child process
    execvp(cmd, args);
} else {
    // Parent process
    waitpid(pid, &status, 0);
}
```

**Notes:**
- Child gets copy of parent's memory (copy-on-write)
- File descriptors are duplicated
- Child has different PID
- Typical overhead: 100-200μs

---

### execvp(3)

Replace process image with new program.

```c
#include <unistd.h>
int execvp(const char *file, char *const argv[]);
```

**Returns:**
- Never returns on success
- -1 on error

**Usage:**
```c
char *args[] = {"ls", "-la", NULL};
execvp("ls", args);
// Only reached if execvp fails
perror("execvp");
exit(127);
```

**Notes:**
- Searches PATH for executable
- argv[0] should be program name
- Last element must be NULL
- Process ID unchanged
- Open file descriptors preserved (unless FD_CLOEXEC)

---

### waitpid(2)

Wait for child process state change.

```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
```

**Parameters:**
- `pid > 0`: Wait for specific child
- `pid = -1`: Wait for any child
- `options = 0`: Block until child exits
- `options = WNOHANG`: Return immediately if no child exited

**Returns:**
- PID of terminated child
- 0 if WNOHANG and no child exited
- -1 on error

**Status Macros:**
```c
WIFEXITED(status)     // True if exited normally
WEXITSTATUS(status)   // Extract exit code
WIFSIGNALED(status)   // True if killed by signal
WTERMSIG(status)      // Extract signal number
```

**Usage:**
```c
int status;
waitpid(pid, &status, 0);
if (WIFEXITED(status)) {
    printf("Exit code: %d\n", WEXITSTATUS(status));
}
```

**Notes:**
- Prevents zombie processes
- Blocking by default
- Use WNOHANG for non-blocking check

---

## Inter-Process Communication

### pipe(2)

Create unidirectional data channel.

```c
#include <unistd.h>
int pipe(int pipefd[2]);
```

**Returns:**
- 0 on success
- -1 on error

**Parameters:**
- `pipefd[0]`: Read end
- `pipefd[1]`: Write end

**Usage:**
```c
int pipefd[2];
pipe(pipefd);

if (fork() == 0) {
    // Writer
    close(pipefd[0]);
    write(pipefd[1], "data", 4);
    close(pipefd[1]);
    exit(0);
} else {
    // Reader
    close(pipefd[1]);
    char buf[100];
    read(pipefd[0], buf, 100);
    close(pipefd[0]);
}
```

**Notes:**
- Buffer size: 64KB on Linux
- Blocking: write blocks when full, read blocks when empty
- Close unused ends to prevent deadlock
- EOF when all write ends closed

---

### dup2(2)

Duplicate file descriptor to specific number.

```c
#include <unistd.h>
int dup2(int oldfd, int newfd);
```

**Returns:**
- newfd on success
- -1 on error

**Usage:**
```c
// Redirect stdout to file
int fd = open("out.txt", O_WRONLY | O_CREAT, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);

// Connect stdout to pipe
dup2(pipefd[1], STDOUT_FILENO);
close(pipefd[1]);
```

**Notes:**
- Atomically closes newfd if open, then duplicates oldfd
- oldfd must be valid file descriptor
- If oldfd == newfd, does nothing and returns newfd

---

## File Operations

### open(2)

Open file and return file descriptor.

```c
#include <fcntl.h>
int open(const char *pathname, int flags, mode_t mode);
```

**Flags:**
- `O_RDONLY`: Read-only
- `O_WRONLY`: Write-only
- `O_RDWR`: Read-write
- `O_CREAT`: Create if doesn't exist
- `O_TRUNC`: Truncate to zero length
- `O_APPEND`: Append to end

**Usage:**
```c
// Input redirection
int fd = open("input.txt", O_RDONLY);

// Output redirection (overwrite)
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

// Append mode
int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
```

**Returns:**
- File descriptor (>= 0) on success
- -1 on error

---

### close(2)

Close file descriptor.

```c
#include <unistd.h>
int close(int fd);
```

**Returns:**
- 0 on success
- -1 on error

**Notes:**
- Always close FDs after use
- Kernel limits FDs per process (typically 1024)
- Closing decrements reference count

---

## Directory Operations

### chdir(2)

Change current working directory.

```c
#include <unistd.h>
int chdir(const char *path);
```

**Returns:**
- 0 on success
- -1 on error

**Usage:**
```c
if (chdir("/tmp") < 0) {
    perror("chdir");
}
```

---

### getcwd(3)

Get current working directory.

```c
#include <unistd.h>
char *getcwd(char *buf, size_t size);
```

**Returns:**
- Pointer to buf on success
- NULL on error

**Usage:**
```c
char cwd[1024];
if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
}
```

---

## Signal Handling

### sigaction(2)

Examine and change signal handler.

```c
#include <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

**Usage:**
```c
struct sigaction sa;
sa.sa_handler = handler_function;
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART;  // Restart interrupted syscalls
sigaction(SIGINT, &sa, NULL);
```

**Common Signals:**
- SIGINT (2): Interrupt (Ctrl+C)
- SIGTSTP (20): Terminal stop (Ctrl+Z)
- SIGCHLD (17): Child process terminated
- SIGTERM (15): Termination request

**Notes:**
- More portable than signal(2)
- Consistent behavior across systems
- SA_RESTART prevents EINTR errors

---

## Common Patterns

### Pipe Between Commands

```c
int pipefd[2];
pipe(pipefd);

// Child 1: cmd1 | ...
if (fork() == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    execvp("cmd1", args1);
}

// Child 2: ... | cmd2
if (fork() == 0) {
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    execvp("cmd2", args2);
}

// Parent
close(pipefd[0]);
close(pipefd[1]);
wait(NULL);
wait(NULL);
```

### Output Redirection

```c
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fork() == 0) {
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execvp("cmd", args);
}
close(fd);
wait(NULL);
```

---

## Error Handling

Always check return values:

```c
if (fork() < 0) {
    perror("fork");
    return -1;
}

if (pipe(pipefd) < 0) {
    perror("pipe");
    return -1;
}

if (dup2(fd, STDOUT_FILENO) < 0) {
    perror("dup2");
    exit(1);
}
```

Use `errno` for detailed error codes:

```c
#include <errno.h>
if (open("file", O_RDONLY) < 0) {
    if (errno == ENOENT) {
        fprintf(stderr, "File not found\n");
    } else if (errno == EACCES) {
        fprintf(stderr, "Permission denied\n");
    }
}
```

---

## References

- `man 2 fork`
- `man 2 execve`
- `man 2 waitpid`
- `man 2 pipe`
- `man 2 dup2`
- Stevens & Rago, *Advanced Programming in the UNIX Environment*
- Kerrisk, *The Linux Programming Interface*
