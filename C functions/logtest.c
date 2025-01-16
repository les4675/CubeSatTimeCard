#include "logging.h"

int main() {
    init_logger("app.log");

    log_message("test1");
    log_message("test2");

    close_logger();

    return;
}