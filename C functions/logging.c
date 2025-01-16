#include "logging.h"
#include <time.h>
#include <stdlib.h>

//Static file pointer for the log file
static FILE *log_file = NULL;

// Function to initialize the logger
void init_logger(const char *filename){
    log_file = fopen(filename, "a+");
    if (log_file == NULL) {
        perror("Error opening log file");
        //exit(EXIT_FAILURE); // Exit the program if the log file cannot be opened
    }
}

// Function to log a message with a timestamp

void log_message(const char *message) {
    if (log_file == NULL) {
        fprintf(stderr, "Logger not initialized. Call init_logger() first.\n");
        return;
    }

    // Get the current timestamp
    char timestamp[20];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);    

    // Write the message to the log file
    fprintf(log_file, "[%s] %s\n", timestamp, message);

    // Flush to ensure it is written immediately
    fflush(log_file);
}

