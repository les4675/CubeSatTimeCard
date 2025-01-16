#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

// Function declarations
void init_logger(const char *filename);
void log_message(const char *message);
void close_logger(void);


#endif