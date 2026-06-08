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

#ifndef SIGNALS_H
#define SIGNALS_H

#include "common.h"

/* Setup signal handlers */
void setup_signals(void);

/* Signal handler for SIGINT (Ctrl+C) */
void sigint_handler(int sig);

/* Signal handler for SIGTSTP (Ctrl+Z) */
void sigtstp_handler(int sig);

#endif /* SIGNALS_H */
