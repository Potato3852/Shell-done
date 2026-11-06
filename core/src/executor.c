#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include "executor.h"

#define COLOR_RED "\033[38;2;255;0;0m"
#define COLOR_RESET "\033[0m"

void execute_command(command_t *cmd) {
    if (cmd->args[0] == NULL) {
        return;
    }

    if (strcmp(cmd->args[0], "cd") == 0) {
        if (cmd->args[1] != NULL && cmd->args[2] != NULL) {
            printf(COLOR_RED "cd: too many arguments\n" COLOR_RESET);
        } else {
            char *target_dir = cmd->args[1];
            if (target_dir == NULL) {
                target_dir = getenv("HOME");
                if (target_dir == NULL) {
                    printf(COLOR_RED "cd: HOME environment variable not set\n" COLOR_RESET);
                    return;
                }
            }

            if (chdir(target_dir) != 0) {
                printf(COLOR_RED "cd: %s: No such file or directory\n" COLOR_RESET, target_dir);
            }
        }
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("Error to create new process!\n");
    } else if (pid == 0) {
        if (cmd->input_file != NULL) {
            int fd = open(cmd->input_file, O_RDONLY);
            if (fd < 0) {
                printf(COLOR_RED "Error opening file for input redirection\n" COLOR_RESET);
                _exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (cmd->output_file != NULL) {
            int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                printf(COLOR_RED "Error opening file for output redirection\n" COLOR_RESET);
                _exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        if (execvp(cmd->args[0], cmd->args) == -1) {
            printf(COLOR_RED "Error to run command/Unknown command!\n" COLOR_RESET);
            _exit(-1);
        }

        _exit(0);

    } else {
        int status;
        signal(SIGINT, SIG_IGN);
        waitpid(pid, &status, 0);
        signal(SIGINT, SIG_DFL);
    }
}