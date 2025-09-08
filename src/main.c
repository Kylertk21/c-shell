#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int fork(void) {

    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;

    child_pid = fork();
    if (child_pid == 0) { // Child PS
        printf("### Child ###\nCurrent PID: %d and Child PID: %d\n",
            getpid(), child_pid);
        sleep(5);
    } else { // Parent PS
        wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        printf("### Parent ###\nCurrent PID: %d and Child PID: %d\n",
            getpid(), child_pid);
    }

    return 0;
}


char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}




int main(void) {

    char *argv[] = {"ls", "-l", "-h", "-a", NULL};
    execvp(argv[0], argv); // name of command & args passed

    return 0;

}