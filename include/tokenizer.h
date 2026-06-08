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

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "common.h"

/* Tokenize input string into array of tokens */
char **tokenize(const char *input, int *token_count);

/* Free token array */
void free_tokens(char **tokens, int token_count);

#endif /* TOKENIZER_H */
