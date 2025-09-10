#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>

char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    if (command == NULL) {
        perror("malloc failed, insufficient memory");
        exit(1);
    }
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);

        if (index > 8) {
            printf("max arguments passed");
        }

    }

    command[index] = NULL;
    return command;
}

int cd(char *path) {
    return chdir(path);
}

int main(void) {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;

    while (1) {
        input = readline("unixsh> ");
        command = get_input(input);

        if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) < 0) {
                perror(command[1]);
            }

            continue; //skip fork
        }

        child_pid = fork();

        if (child_pid < 0) {
            perror("Fork Failed");
            exit(1);
        }

        if (child_pid == 0) { // Child Process
            /* Never returns if call is successful */
            execvp(command[0], command);
            if (execvp(command[0], command) < 0) {
                perror(command[0]);
                exit(1);
            }

            printf("Program exited, execvp unsuccessful");
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }
        free(input);
        free(command);
    }
    return 0;
}