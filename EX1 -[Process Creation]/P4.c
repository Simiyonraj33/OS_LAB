#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork Failed");
        return 1;
    }

    if (pid == 0) {
        int sum = 0;
        for (int i = 1; i <= n; i++) {
            sum += i;
        }
        printf("\n--- Child Process ---\n");
        printf("Sum of first %d natural numbers = %d\n", n, sum);
    } else {
        wait(NULL);
        printf("\n--- Parent Process ---\n");
        printf("Collatz Sequence: ");
        while (n != 1) {
            printf("%d ", n);
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
        }
        printf("1\n");
    }

    return 0;
}