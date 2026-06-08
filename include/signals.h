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
