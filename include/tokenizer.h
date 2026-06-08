#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "common.h"

/* Tokenize input string into array of tokens */
char **tokenize(const char *input, int *token_count);

/* Free token array */
void free_tokens(char **tokens, int token_count);

#endif /* TOKENIZER_H */
