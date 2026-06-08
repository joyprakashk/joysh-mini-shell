#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

/* Initialize logger */
int init_logger(void);

/* Log executed command */
void log_command(const char *cmd, int success);

/* Close logger */
void close_logger(void);

#endif /* LOGGER_H */
