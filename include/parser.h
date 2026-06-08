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
