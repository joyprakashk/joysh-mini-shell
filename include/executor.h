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

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "common.h"

/* Execute single command */
int execute_command(Command *cmd);

/* Execute pipeline (multiple commands connected by pipes) */
int execute_pipeline(Command *commands, int cmd_count);

#endif /* EXECUTOR_H */
