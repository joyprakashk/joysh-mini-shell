#include "../include/tokenizer.h"
#include <ctype.h>

char **tokenize(const char *input, int *token_count) {
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    if (!tokens) return NULL;
    
    int idx = 0;
    const char *ptr = input;
    
    while (*ptr && idx < MAX_TOKENS) {
        /* Skip whitespace */
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        /* Allocate token */
        char *token = malloc(MAX_TOKEN_SIZE);
        if (!token) {
            free_tokens(tokens, idx);
            return NULL;
        }
        
        int token_len = 0;
        
        /* Handle quoted strings */
        if (*ptr == '"' || *ptr == '\'') {
            char quote = *ptr++;
            while (*ptr && *ptr != quote && token_len < MAX_TOKEN_SIZE - 1) {
                token[token_len++] = *ptr++;
            }
            if (*ptr == quote) ptr++;
        }
        /* Handle special characters */
        else if (*ptr == '|' || *ptr == '<' || *ptr == '>' || *ptr == '&') {
            token[token_len++] = *ptr++;
            /* Handle >> and 2> */
            if (*(ptr - 1) == '>' && *ptr == '>') {
                token[token_len++] = *ptr++;
            } else if (*(ptr - 1) == '2' && *ptr == '>') {
                token[0] = '2';
                token[1] = '>';
                token_len = 2;
                ptr++;
            }
        }
        /* Handle regular tokens */
        else {
            while (*ptr && !isspace(*ptr) && *ptr != '|' && *ptr != '<' && 
                   *ptr != '>' && *ptr != '&' && token_len < MAX_TOKEN_SIZE - 1) {
                token[token_len++] = *ptr++;
            }
        }
        
        token[token_len] = '\0';
        tokens[idx++] = token;
    }
    
    *token_count = idx;
    return tokens;
}

void free_tokens(char **tokens, int token_count) {
    if (!tokens) return;
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
