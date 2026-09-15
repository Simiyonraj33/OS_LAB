#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;

    pid1 = fork();
    if (pid1 == 0) {
        printf("P4 -> PID = %d, Parent = %d\n", getpid(), getppid());
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            printf("P2 -> PID = %d Parent = %d\n", getpid(), getppid());

            pid3 = fork();
            if (pid3 == 0) {
                printf("P3 -> PID = %d Parent = %d\n", getpid(), getppid());
            } else {
                wait(NULL);
            }
        } else {
            wait(NULL);
            wait(NULL);
            printf("P1 -> PID = %d\n", getpid());
        }
    }

    return 0;
}