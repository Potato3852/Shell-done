#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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

        char *token = strtok(input_buffer, " \t\n");

        while (token != NULL && i < 63) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }

        args[i] = NULL;

        pid_t pid = fork();

        if (pid < 0) {
            printf("Error to create new process!\n");
            break;
        } else if (pid == 0) {
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