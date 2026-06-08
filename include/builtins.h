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

#ifndef BUILTINS_H
#define BUILTINS_H

#include "common.h"

/* Execute built-in command */
int execute_builtin(Command *cmd);

/* Built-in: cd */
int builtin_cd(char **args);

/* Built-in: pwd */
int builtin_pwd(void);

/* Built-in: exit */
int builtin_exit(char **args);

/* Built-in: clear */
int builtin_clear(void);

/* Built-in: history */
int builtin_history(void);

/* Built-in: stats */
int builtin_stats(void);

/* Add command to history */
void add_to_history(const char *cmd);

#endif /* BUILTINS_H */
