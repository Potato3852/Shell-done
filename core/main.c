#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 1024

void shell_cleanup(void) {
    printf("exit...");
}

int main(int argc, char* argv[]) {

    char input_buffer[MAX_LINE];

    if(atexit(shell_cleanup) != 0) {
        printf("Error to register cleanup function!");
        return 1;
    }
    
    while(1) {
        printf("> ");

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

        printf("you enter: \"%s\", but now i cannot to handle this(\n", input_buffer);
    }

    exit(0);
}