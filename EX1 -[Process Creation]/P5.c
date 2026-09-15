#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t m1, m2, w1, w2, w3;

    printf("P (Root Process)\n");
    printf("PID : %d | Parent PID : %d\n\n", getpid(), getppid());

    m1 = fork();
    if (m1 < 0) {
        perror("Fork M1 Failed");
        return 1;
    } else if (m1 == 0) {
        printf("M1 (Child of P)\n");
        printf("PID : %d | Parent PID : %d\n\n", getpid(), getppid());

        w1 = fork();
        if (w1 < 0) {
            perror("Fork W1 Failed");
            return 1;
        } else if (w1 == 0) {
            printf("W1 (Child of M1)\n");
            printf("PID : %d | Parent PID : %d\n\n", getpid(), getppid());
            return 0;
        }
        wait(NULL);

        w2 = fork();
        if (w2 < 0) {
            perror("Fork W2 Failed");
            return 1;
        } else if (w2 == 0) {
            printf("W2 (Child of M1)\n");
            printf("PID : %d | Parent PID : %d\n\n", getpid(), getppid());
            return 0;
        }
        wait(NULL);
        return 0;

    } else {
        wait(NULL);

        m2 = fork();
        if (m2 < 0) {
            perror("Fork M2 Failed");
            return 1;
        } else if (m2 == 0) {
            printf("M2 (Child of P)\n");
            printf("PID : %d | Parent PID : %d\n\n", getpid(), getppid());

            w3 = fork();
            if (w3 < 0) {
                perror("Fork W3 Failed");
                return 1;
            } else if (w3 == 0) {
                printf("W3 (Child of M2)\n");
                printf("PID : %d | Parent PID : %d\n\n", getpid(), getppid());
                return 0;
            }
            wait(NULL);
            return 0;
        } else {
            wait(NULL);
        }
    }

    return 0;
}