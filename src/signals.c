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

#include "../include/signals.h"

void sigint_handler(int sig) {
    /* Ctrl+C pressed - ignore in parent shell, let child handle it */
    printf("\n");
    fflush(stdout);
}

void sigtstp_handler(int sig) {
    /* Ctrl+Z pressed - ignore in parent shell */
    printf("\n");
    fflush(stdout);
}

void setup_signals(void) {
    struct sigaction sa_int, sa_tstp;
    
    /* Setup SIGINT handler */
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    
    /* Setup SIGTSTP handler */
    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL);
}
