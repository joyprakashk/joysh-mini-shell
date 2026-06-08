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

#ifndef REDIRECT_H
#define REDIRECT_H

#include "common.h"

/* Setup input redirection */
int setup_input_redirect(const char *filename);

/* Setup output redirection */
int setup_output_redirect(const char *filename, int append);

/* Setup stderr redirection */
int setup_stderr_redirect(const char *filename);

/* Restore stdin/stdout after redirection */
void restore_stdin(int saved_stdin);
void restore_stdout(int saved_stdout);

#endif /* REDIRECT_H */
