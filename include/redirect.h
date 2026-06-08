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
