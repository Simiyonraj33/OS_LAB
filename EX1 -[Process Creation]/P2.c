#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid1 == 0) {
        printf("P2 : PID = %d Parent = %d\n", getpid(), getppid());

        pid2 = fork();
        if (pid2 < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid2 == 0) {
            printf("P3 : PID = %d Parent = %d\n", getpid(), getppid());
        } else {
            wait(NULL);
        }
    } else {
        wait(NULL);
        printf("P1 : PID = %d\n", getpid());
    }

    return 0;
}