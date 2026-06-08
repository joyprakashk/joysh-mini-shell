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

#ifndef PARSER_H
#define PARSER_H

#include "common.h"

/* Parse tokens into command structure */
Command *parse_command(char **tokens, int token_count, int *cmd_count);

/* Free command structure */
void free_commands(Command *commands, int cmd_count);

/* Check if command is a built-in */
int is_builtin(const char *cmd);

#endif /* PARSER_H */
