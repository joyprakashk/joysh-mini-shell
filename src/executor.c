#include "../include/executor.h"
#include "../include/redirect.h"
#include "../include/builtins.h"
#include "../include/parser.h"

int execute_command(Command *cmd) {
    if (!cmd || !cmd->args || !cmd->args[0]) return -1;
    
    /* Handle built-in commands */
    if (is_builtin(cmd->args[0])) {
        return execute_builtin(cmd);
    }
    
    /* Fork for external command */
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        /* Child process */
        
        /* Setup redirections */
        if (cmd->input_file) {
            if (setup_input_redirect(cmd->input_file) < 0) exit(1);
        }
        
        if (cmd->output_file) {
            if (cmd->stderr_redirect) {
                if (setup_stderr_redirect(cmd->output_file) < 0) exit(1);
            } else {
                if (setup_output_redirect(cmd->output_file, cmd->append) < 0) exit(1);
            }
        }
        
        /* Execute command */
        execvp(cmd->args[0], cmd->args);
        
        /* If execvp returns, it failed */
        fprintf(stderr, "joysh: %s: command not found\n", cmd->args[0]);
        exit(127);
    } else {
        /* Parent process */
        if (!cmd->background) {
            int status;
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            }
        } else {
            printf("[Background] PID: %d\n", pid);
            g_stats.background_jobs++;
        }
    }
    
    return 0;
}

int execute_pipeline(Command *commands, int cmd_count) {
    if (cmd_count == 1) {
        return execute_command(&commands[0]);
    }
    
    /* Track pipe count for stats */
    g_stats.pipes_executed++;
    
    int pipes[cmd_count - 1][2];
    pid_t pids[cmd_count];
    
    /* Create all pipes */
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return -1;
        }
    }
    
    /* Fork and execute each command */
    for (int i = 0; i < cmd_count; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            return -1;
        } else if (pids[i] == 0) {
            /* Child process */
            
            /* Setup input from previous pipe */
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            
            /* Setup output to next pipe */
            if (i < cmd_count - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            /* Close all pipe file descriptors */
            for (int j = 0; j < cmd_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            /* Setup file redirections */
            if (commands[i].input_file) {
                if (setup_input_redirect(commands[i].input_file) < 0) exit(1);
            }
            
            if (commands[i].output_file) {
                if (setup_output_redirect(commands[i].output_file, commands[i].append) < 0) exit(1);
            }
            
            /* Execute command */
            execvp(commands[i].args[0], commands[i].args);
            
            /* If execvp returns, it failed */
            fprintf(stderr, "joysh: %s: command not found\n", commands[i].args[0]);
            exit(127);
        }
    }
    
    /* Parent: close all pipes */
    for (int i = 0; i < cmd_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    /* Wait for all children if not background */
    if (!commands[cmd_count - 1].background) {
        for (int i = 0; i < cmd_count; i++) {
            int status;
            waitpid(pids[i], &status, 0);
        }
    } else {
        printf("[Background Pipeline] PIDs:");
        for (int i = 0; i < cmd_count; i++) {
            printf(" %d", pids[i]);
        }
        printf("\n");
        g_stats.background_jobs++;
    }
    
    return 0;
}
