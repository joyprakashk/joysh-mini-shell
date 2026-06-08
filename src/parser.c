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

#include "../include/parser.h"

int is_builtin(const char *cmd) {
    if (!cmd) return 0;
    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") == 0 || 
            strcmp(cmd, "exit") == 0 || strcmp(cmd, "clear") == 0 ||
            strcmp(cmd, "history") == 0 || strcmp(cmd, "stats") == 0);
}

Command *parse_command(char **tokens, int token_count, int *cmd_count) {
    if (token_count == 0) {
        *cmd_count = 0;
        return NULL;
    }
    
    /* Count pipes to determine number of commands */
    int pipe_count = 0;
    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], "|") == 0) pipe_count++;
    }
    
    int num_commands = pipe_count + 1;
    Command *commands = calloc(num_commands, sizeof(Command));
    if (!commands) return NULL;
    
    int cmd_idx = 0;
    int arg_start = 0;
    int background = 0;
    
    /* Check for background execution */
    if (strcmp(tokens[token_count - 1], "&") == 0) {
        background = 1;
        token_count--;
    }
    
    for (int i = 0; i <= token_count; i++) {
        /* Parse command at pipe boundary or end */
        if (i == token_count || strcmp(tokens[i], "|") == 0) {
            int arg_count = 0;
            commands[cmd_idx].args = malloc((i - arg_start + 1) * sizeof(char *));
            
            for (int j = arg_start; j < i; j++) {
                /* Handle redirections */
                if (strcmp(tokens[j], "<") == 0 && j + 1 < i) {
                    commands[cmd_idx].input_file = strdup(tokens[++j]);
                } else if (strcmp(tokens[j], ">") == 0 && j + 1 < i) {
                    commands[cmd_idx].output_file = strdup(tokens[++j]);
                    commands[cmd_idx].append = 0;
                } else if (strcmp(tokens[j], ">>") == 0 && j + 1 < i) {
                    commands[cmd_idx].output_file = strdup(tokens[++j]);
                    commands[cmd_idx].append = 1;
                } else if (strcmp(tokens[j], "2>") == 0 && j + 1 < i) {
                    commands[cmd_idx].output_file = strdup(tokens[++j]);
                    commands[cmd_idx].stderr_redirect = 1;
                } else {
                    commands[cmd_idx].args[arg_count++] = strdup(tokens[j]);
                }
            }
            commands[cmd_idx].args[arg_count] = NULL;
            commands[cmd_idx].background = (i == token_count && background);
            
            cmd_idx++;
            arg_start = i + 1;
        }
    }
    
    *cmd_count = num_commands;
    return commands;
}

void free_commands(Command *commands, int cmd_count) {
    if (!commands) return;
    
    for (int i = 0; i < cmd_count; i++) {
        if (commands[i].args) {
            for (int j = 0; commands[i].args[j] != NULL; j++) {
                free(commands[i].args[j]);
            }
            free(commands[i].args);
        }
        if (commands[i].input_file) free(commands[i].input_file);
        if (commands[i].output_file) free(commands[i].output_file);
    }
    free(commands);
}
