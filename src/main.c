#include "../include/common.h"
#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/executor.h"
#include "../include/builtins.h"
#include "../include/signals.h"
#include "../include/logger.h"

/* Global stats */
ShellStats g_stats = {0};

void print_prompt(void) {
    char cwd[MAX_PATH];
    char hostname[64];
    
    getcwd(cwd, sizeof(cwd));
    gethostname(hostname, sizeof(hostname));
    
    printf(COLOR_GREEN "joysh" COLOR_RESET 
           COLOR_CYAN "[%s@%s]" COLOR_RESET 
           COLOR_YELLOW "%s" COLOR_RESET "$ ", 
           getenv("USER") ? getenv("USER") : "user", 
           hostname, 
           cwd);
    fflush(stdout);
}

int main(void) {
    char *input = NULL;
    size_t input_size = 0;
    
    /* Initialize */
    g_stats.start_time = time(NULL);
    setup_signals();
    init_logger();
    
    printf(COLOR_CYAN "╔════════════════════════════════════════╗\n");
    printf("║          Welcome to joysh              ║\n");
    printf("║   A production-grade UNIX shell in C   ║\n");
    printf("╚════════════════════════════════════════╝\n" COLOR_RESET);
    printf("Type 'help' for commands, 'exit' to quit\n\n");
    
    /* Main shell loop */
    while (1) {
        print_prompt();
        
        /* Read input */
        ssize_t read = getline(&input, &input_size, stdin);
        
        if (read == -1) {
            /* EOF (Ctrl+D) */
            printf("\n");
            break;
        }
        
        /* Remove newline */
        if (input[read - 1] == '\n') {
            input[read - 1] = '\0';
        }
        
        /* Skip empty input */
        if (strlen(input) == 0) continue;
        
        /* Add to history */
        add_to_history(input);
        
        /* Tokenize */
        int token_count;
        char **tokens = tokenize(input, &token_count);
        
        if (!tokens || token_count == 0) {
            free_tokens(tokens, token_count);
            continue;
        }
        
        /* Parse */
        int cmd_count;
        Command *commands = parse_command(tokens, token_count, &cmd_count);
        free_tokens(tokens, token_count);
        
        if (!commands || cmd_count == 0) {
            free_commands(commands, cmd_count);
            continue;
        }
        
        /* Execute */
        int status = execute_pipeline(commands, cmd_count);
        
        /* Update stats */
        g_stats.total_commands++;
        if (status != 0) {
            g_stats.failed_commands++;
            log_command(input, 0);
        } else {
            log_command(input, 1);
        }
        
        /* Cleanup */
        free_commands(commands, cmd_count);
    }
    
    /* Cleanup */
    free(input);
    close_logger();
    
    return 0;
}
