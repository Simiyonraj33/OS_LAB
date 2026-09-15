#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("P2 : PID = %d Parent = %d\n", getpid(), getppid());
    }
    if (pid > 0) {
        wait(NULL);
        printf("P1 : PID = %d Parent = %d\n", getpid(), getppid());
    }
    return 0;
}