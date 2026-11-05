#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_LINE 1024

#define COLOR_RED     "\033[38;2;255;0;0m"
#define COLOR_GREEN   "\033[38;2;0;255;127m"
#define COLOR_RESET   "\033[0m"

void shell_cleanup(void) {
    printf("exit...");
}

int main(int argc, char* argv[]) {

    char input_buffer[MAX_LINE];

    if(atexit(shell_cleanup) != 0) {
        printf("Error to register cleanup function!");
        return 1;
    }

    int status;
    
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

        char* args[64];
        int i = 0;
        char* output_file = NULL;
        char* input_file = NULL;

        char *token = strtok(input_buffer, " \t\n");

        while (token != NULL && i < 63) {
            if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " \t\n");
                if (token != NULL) {
                    output_file = token;
                } else {
                    printf(COLOR_RED "syntax error near unexpected token `newline'\n" COLOR_RESET);
                    break;
                }
            } else if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " \t\n");
                if (token != NULL) {
                    input_file = token;
                } else {
                    printf(COLOR_RED "syntax error near unexpected token `newline'\n" COLOR_RESET);
                    break;
                }
            } else {
                args[i++] = token;
            }
            
            token = strtok(NULL, " \t\n");
        }

        args[i] = NULL;

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] != NULL && args[2] != NULL) {
                printf(COLOR_RED "cd: too many arguments\n" COLOR_RESET);
            } else {
                char *target_dir = args[1];
                if (target_dir == NULL) {
                    target_dir = getenv("HOME");
                    if (target_dir == NULL) {
                        printf(COLOR_RED "cd: HOME environment variable not set\n" COLOR_RESET);
                        continue;
                    }
                }

                if (chdir(target_dir) != 0) {
                    printf(COLOR_RED "cd: %s: No such file or directory\n" COLOR_RESET, target_dir);
                }
            }
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            printf("Error to create new process!\n");
            break;
        } else if (pid == 0) {
            if (input_file != NULL) {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0) {
                    printf(COLOR_RED "Error opening file for input redirection\n" COLOR_RESET);
                    _exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (output_file != NULL) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    printf(COLOR_RED "Error opening file for output redirection\n" COLOR_RESET);
                    _exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            if (execvp(args[0], args) == -1) {
                printf(COLOR_RED "Error to run command/Unkhown command!\n" COLOR_RESET);
                _exit(-1);
            }

            _exit(0);

        } else {
            signal(SIGINT, SIG_IGN);
            waitpid(pid, &status, 0);
            signal(SIGINT, SIG_DFL);
        }
    }

    exit(0);
}