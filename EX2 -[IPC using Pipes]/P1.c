#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int no, temp;

    pipe(fd);

    if (fork() == 0)
    {
        // Child process
        printf("Child Process\n");
        printf("Enter a number: ");
        scanf("%d", &no);

        write(fd[1], &no, sizeof(no));
        close(fd[1]);
    }
    else
    {
        // Parent process
        wait(NULL);

        read(fd[0], &no, sizeof(no));
        close(fd[0]);

        temp = no;

        while (temp > 1)
        {
            if (temp % 3 != 0)
                break;

            temp = temp / 3;
        }

        if (temp == 1)
            printf("Parent: %d is a power of 3\n", no);
        else
            printf("Parent: %d is not a power of 3\n", no);
    }

    return 0;
}