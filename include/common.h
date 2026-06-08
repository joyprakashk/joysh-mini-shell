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

#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

/* Constants */
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_TOKENS 64
#define MAX_HISTORY 100
#define MAX_PATH 256

/* ANSI Color Codes */
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_YELLOW  "\033[1;33m"

/* Command Structure */
typedef struct {
    char **args;           /* Command arguments */
    char *input_file;      /* Input redirection file */
    char *output_file;     /* Output redirection file */
    int append;            /* Append mode for output */
    int background;        /* Background execution flag */
    int stderr_redirect;   /* Stderr redirection flag */
} Command;

/* Global Statistics */
typedef struct {
    int total_commands;
    int background_jobs;
    int pipes_executed;
    int failed_commands;
    time_t start_time;
} ShellStats;

extern ShellStats g_stats;

#endif /* COMMON_H */
