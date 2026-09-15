#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int p1[2], p2[2];
    char str[100];
    char status[20];

    pipe(p1);
    pipe(p2);

    if (fork() == 0)
    {
        // Child process
        int i, n = 0, sp = 0;

        read(p1[0], str, sizeof(str));
        close(p1[0]);

        for (i = 0; str[i] != '\0'; i++)
        {
            if (isdigit(str[i]))
                n = 1;

            if (!isalnum(str[i]))
                sp = 1;
        }

        if (strlen(str) > 7 && n == 1 && sp == 1)
            strcpy(status, "VALID");
        else
            strcpy(status, "INVALID");

        write(p2[1], status, sizeof(status));
        close(p2[1]);
    }
    else
    {
        // Parent process
        printf("Parent Process\n");
        printf("Enter a string: ");
        scanf("%99s", str);

        write(p1[1], str, sizeof(str));
        close(p1[1]);

        wait(NULL);

        read(p2[0], status, sizeof(status));
        close(p2[0]);

        if (strcmp(status, "VALID") == 0)
            printf("The string is VALID\n");
        else
            printf("The string is INVALID\n");
    }

    return 0;
}