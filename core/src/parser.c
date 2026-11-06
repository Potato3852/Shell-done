#include <stdio.h>
#include <string.h>
#include "parser.h"

#define COLOR_RED "\033[38;2;255;0;0m"
#define COLOR_RESET "\033[0m"

void parse_command(char *input_buffer, command_t *cmd) {
    cmd->input_file = NULL;
    cmd->output_file = NULL;

    int i = 0;
    char *token = strtok(input_buffer, " \t\n");

    while (token != NULL && i < 63) {
        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                cmd->output_file = token;
            } else {
                printf(COLOR_RED "syntax error near unexpected token `newline'\n" COLOR_RESET);
                break;
            }
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                cmd->input_file = token;
            } else {
                printf(COLOR_RED "syntax error near unexpected token `newline'\n" COLOR_RESET);
                break;
            }
        } else {
            cmd->args[i++] = token;
        }

        token = strtok(NULL, " \t\n");
    }

    cmd->args[i] = NULL;
}