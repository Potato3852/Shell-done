#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"

#define COLOR_GREEN "\033[38;2;0;255;127m"
#define COLOR_RESET "\033[0m"

void shell_cleanup(void) {
    printf("exit...");
}

int main(void) {

    char input_buffer[MAX_LINE];

    if(atexit(shell_cleanup) != 0) {
        printf("Error to register cleanup function!");
        return 1;
    }
    
    while(1) {
        printf(COLOR_GREEN "> "COLOR_RESET);

        if((fgets(input_buffer, sizeof(input_buffer), stdin)) == NULL) {
            printf("\nexit.\n");
            break;
        }

        input_buffer[strcspn(input_buffer, "\n")] = 0;

        if (strlen(input_buffer) == 0) {
            continue;
        }

        if (strcmp(input_buffer, "exit") == 0) {
            break;
        }

        command_t cmd;
        parse_command(input_buffer, &cmd);
        execute_command(&cmd);
    }

    exit(0);
}