#include "../include/builtins.h"

static char *history[MAX_HISTORY];
static int history_count = 0;

void add_to_history(const char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);
        memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char *));
        history[MAX_HISTORY - 1] = strdup(cmd);
    }
}

int execute_builtin(Command *cmd) {
    if (!cmd || !cmd->args || !cmd->args[0]) return -1;
    
    if (strcmp(cmd->args[0], "cd") == 0) {
        return builtin_cd(cmd->args);
    } else if (strcmp(cmd->args[0], "pwd") == 0) {
        return builtin_pwd();
    } else if (strcmp(cmd->args[0], "exit") == 0) {
        return builtin_exit(cmd->args);
    } else if (strcmp(cmd->args[0], "clear") == 0) {
        return builtin_clear();
    } else if (strcmp(cmd->args[0], "history") == 0) {
        return builtin_history();
    } else if (strcmp(cmd->args[0], "stats") == 0) {
        return builtin_stats();
    }
    
    return -1;
}

int builtin_cd(char **args) {
    const char *path = args[1] ? args[1] : getenv("HOME");
    
    if (chdir(path) != 0) {
        perror("cd");
        return -1;
    }
    return 0;
}

int builtin_pwd(void) {
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    }
    perror("pwd");
    return -1;
}

int builtin_exit(char **args) {
    int exit_code = args[1] ? atoi(args[1]) : 0;
    printf("Exiting joysh...\n");
    exit(exit_code);
}

int builtin_clear(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
    return 0;
}

int builtin_history(void) {
    for (int i = 0; i < history_count; i++) {
        printf("%4d  %s\n", i + 1, history[i]);
    }
    return 0;
}

int builtin_stats(void) {
    time_t uptime = time(NULL) - g_stats.start_time;
    
    printf(COLOR_CYAN "=== joysh Statistics ===\n" COLOR_RESET);
    printf("Total commands executed: %d\n", g_stats.total_commands);
    printf("Background jobs: %d\n", g_stats.background_jobs);
    printf("Pipes executed: %d\n", g_stats.pipes_executed);
    printf("Failed commands: %d\n", g_stats.failed_commands);
    printf("Shell uptime: %ld seconds\n", uptime);
    
    return 0;
}
