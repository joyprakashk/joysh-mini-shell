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
