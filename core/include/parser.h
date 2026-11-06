#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE 1024

typedef struct {
    char *args[64];
    char *input_file;
    char *output_file;
} command_t;

void parse_command(char *input_buffer, command_t *cmd);

#endif