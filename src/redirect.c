/*
 * joysh - Production-grade UNIX shell in C
 *
 * Copyright (C) 2026 Joyprakash Kalita
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the LICENSE file for more details.
 */

#include "../include/redirect.h"

int setup_input_redirect(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("input redirection");
        return -1;
    }
    
    if (dup2(fd, STDIN_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int setup_output_redirect(const char *filename, int append) {
    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("output redirection");
        return -1;
    }
    
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int setup_stderr_redirect(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("stderr redirection");
        return -1;
    }
    
    if (dup2(fd, STDERR_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

void restore_stdin(int saved_stdin) {
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
}

void restore_stdout(int saved_stdout) {
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}
