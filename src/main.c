#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

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