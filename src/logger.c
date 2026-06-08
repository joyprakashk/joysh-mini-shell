#include "../include/logger.h"

static FILE *log_file = NULL;

int init_logger(void) {
    char log_path[MAX_PATH];
    const char *home = getenv("HOME");
    
    if (!home) {
        fprintf(stderr, "Warning: HOME not set, logging disabled\n");
        return -1;
    }
    
    snprintf(log_path, sizeof(log_path), "%s/.joysh_logs", home);
    
    log_file = fopen(log_path, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return -1;
    }
    
    /* Log shell start */
    time_t now = time(NULL);
    fprintf(log_file, "\n=== Shell started at %s", ctime(&now));
    fflush(log_file);
    
    return 0;
}

void log_command(const char *cmd, int success) {
    if (!log_file) return;
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(log_file, "[%s] %s | Status: %s\n", 
            timestamp, cmd, success ? "SUCCESS" : "FAILED");
    fflush(log_file);
}

void close_logger(void) {
    if (log_file) {
        time_t now = time(NULL);
        fprintf(log_file, "=== Shell ended at %s\n", ctime(&now));
        fclose(log_file);
        log_file = NULL;
    }
}
