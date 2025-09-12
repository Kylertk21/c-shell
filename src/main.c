#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <signal.h>
#include <setjmp.h>

static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;
void sigint_handler(int) {
    if (!jump_active) {
        return;
    }
    siglongjmp(env, 42);
}

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

    struct sigaction s;
    s.sa_handler = sigint_handler; // pointer to signal handler
    sigemptyset(&s.sa_mask); // initializes sa_mask with an empty set
    // (optional set of signals blocked while handler is executing)
    s.sa_flags = SA_RESTART; // bitwise OR of config flags, set to restart system if
    // signal handler is invoked in the middle of a system call
    sigaction(SIGINT, &s, NULL); // signal number, new config, backup of current config


    while (1) {
        if (sigsetjmp(env, 1) == 42) {
            printf("\n");
            continue;
        }
        jump_active = 1;

        input = readline("k-shell> ");
        if (input == NULL) { //exit on ctrl-d
            printf("Exiting...\n");
            exit(0);

        }

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
            signal(SIGINT, SIG_DFL); // Default behavior for SIGINT for child ps
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