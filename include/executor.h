#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "common.h"

/* Execute single command */
int execute_command(Command *cmd);

/* Execute pipeline (multiple commands connected by pipes) */
int execute_pipeline(Command *commands, int cmd_count);

#endif /* EXECUTOR_H */
