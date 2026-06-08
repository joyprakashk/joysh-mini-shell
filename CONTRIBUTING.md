# Contributing

## Getting Started

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/name`)
3. Make changes
4. Build and test (`make && ./tests/test.sh`)
5. Commit (`git commit -am 'Add feature'`)
6. Push (`git push origin feature/name`)
7. Create Pull Request

## Code Style

- C11 standard
- 4-space indentation
- No tabs
- Line length: 100 characters max
- Function names: `snake_case`
- Struct names: `PascalCase`
- Constants: `UPPER_SNAKE_CASE`

## Building

```bash
make clean
make
```

## Testing

All changes must pass existing tests:

```bash
./tests/test.sh
```

Add tests for new features in `tests/test.sh`.

## Memory Management

- No leaks (verify with `make valgrind`)
- Free all allocated memory
- Close all file descriptors

## Adding Features

### New Built-in Command

1. Add declaration to `include/builtins.h`
2. Implement in `src/builtins.c`
3. Register in `is_builtin()` in `src/parser.c`
4. Add case in `execute_builtin()` in `src/builtins.c`
5. Add test case

Example:
```c
// include/builtins.h
int builtin_help(void);

// src/builtins.c
int builtin_help(void) {
    printf("Available commands: cd, pwd, exit, clear, history, stats\n");
    return 0;
}

// src/parser.c (is_builtin)
return (strcmp(cmd, "cd") == 0 || ... || strcmp(cmd, "help") == 0);

// src/builtins.c (execute_builtin)
else if (strcmp(cmd->args[0], "help") == 0) {
    return builtin_help();
}
```

### New System Feature

1. Define interface in appropriate header
2. Implement in corresponding .c file
3. Update relevant components
4. Add documentation
5. Add tests

## Commit Messages

Format:
```
type: brief description

Longer explanation if needed.

Fixes #issue
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation
- `refactor`: Code restructuring
- `test`: Testing
- `chore`: Maintenance

Examples:
```
feat: add job control (fg/bg)

Implements job control with process group management.
Tracks background jobs in job table.

Fixes #42
```

```
fix: prevent FD leak in pipe execution

Close unused pipe ends in parent process.
```

## Pull Requests

- One feature per PR
- Include tests
- Update documentation
- No compiler warnings
- Passes `make valgrind`

## Code Review

All PRs require:
- Clean build (`make`)
- Passing tests (`./tests/test.sh`)
- No memory leaks (`make valgrind`)
- Code style compliance
- Documentation updates

## Questions

Open an issue for:
- Bug reports
- Feature requests
- Design questions
- Implementation help

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
