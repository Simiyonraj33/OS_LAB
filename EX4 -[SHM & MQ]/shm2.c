#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX_SIZE 100

struct shared_data
{
    int numbers[MAX_SIZE];
    int total_sum;
    int n;
};

// Function to calculate sum of a process slice
int calculate_slice_sum(int *arr, int start, int count)
{
    int sum = 0;

    for (int i = start; i < start + count; i++)
    {
        sum += arr[i];
    }

    return sum;
}

int main()
{
    key_t key = 5678;

    // Allocate shared memory
    int shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);

    if (shmid < 0)
    {
        perror("shmget failed");
        exit(1);
    }

    // Attach shared memory
    struct shared_data *shm =
        (struct shared_data *)shmat(shmid, NULL, 0);

    if (shm == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    // Initialize total
    shm->total_sum = 0;

    // Input
    int n;

    do
    {
        printf("[P1] Enter total number of elements (n) [Must be divisible by 4]: ");
        scanf("%d", &n);

        if (n <= 0 || n % 4 != 0 || n > MAX_SIZE)
        {
            printf("--> Error: n must be positive, divisible by 4 and <= 100.\n");
        }

    } while (n <= 0 || n % 4 != 0 || n > MAX_SIZE);

    shm->n = n;

    printf("[P1] Enter %d integer elements: ", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &shm->numbers[i]);
    }

    int slice = n / 4;

    // ================= P1 -> P2 -> P3 -> P4 =================

    pid_t pid2 = fork();

    if (pid2 < 0)
    {
        perror("fork failed");
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    if (pid2 == 0)
    {
        // ================= P2 =================

        pid_t pid3 = fork();

        if (pid3 < 0)
        {
            perror("fork P3 failed");
            shmdt(shm);
            exit(1);
        }

        if (pid3 == 0)
        {
            // ================= P3 =================

            pid_t pid4 = fork();

            if (pid4 < 0)
            {
                perror("fork P4 failed");
                shmdt(shm);
                exit(1);
            }

            if (pid4 == 0)
            {
                // ================= P4 =================

                int sum = calculate_slice_sum(
                    shm->numbers,
                    3 * slice,
                    slice
                );

                shm->total_sum += sum;

                printf("[P4 (PID %d)] Processing indices %d to %d -> Sum = %d\n",
                       getpid(),
                       3 * slice,
                       (3 * slice) + slice - 1,
                       sum);

                if (shmdt(shm) == -1)
                {
                    perror("P4 shmdt failed");
                }

                exit(0);
            }
            else
            {
                // ================= P3 =================

                int sum = calculate_slice_sum(
                    shm->numbers,
                    2 * slice,
                    slice
                );

                shm->total_sum += sum;

                printf("[P3 (PID %d)] Processing indices %d to %d -> Sum = %d\n",
                       getpid(),
                       2 * slice,
                       (2 * slice) + slice - 1,
                       sum);

                if (wait(NULL) == -1)
                {
                    perror("P3 wait failed");
                }

                if (shmdt(shm) == -1)
                {
                    perror("P3 shmdt failed");
                }

                exit(0);
            }
        }
        else
        {
            // ================= P2 =================

            int sum = calculate_slice_sum(
                shm->numbers,
                1 * slice,
                slice
            );

            shm->total_sum += sum;

            printf("[P2 (PID %d)] Processing indices %d to %d -> Sum = %d\n",
                   getpid(),
                   1 * slice,
                   (1 * slice) + slice - 1,
                   sum);

            if (wait(NULL) == -1)
            {
                perror("P2 wait failed");
            }

            if (shmdt(shm) == -1)
            {
                perror("P2 shmdt failed");
            }

            exit(0);
        }
    }
    else
    {
        // ================= P1 =================

        int sum = calculate_slice_sum(
            shm->numbers,
            0,
            slice
        );

        shm->total_sum += sum;

        printf("\n[P1 (PID %d)] Processing indices %d to %d -> Sum = %d\n",
               getpid(),
               0,
               slice - 1,
               sum);

        if (wait(NULL) == -1)
        {
            perror("P1 wait failed");
        }

        printf("\n================ SUMMARY ================\n");
        printf("[P1] TOTAL SUM = %d\n", shm->total_sum);
        printf("=========================================\n");

        if (shmdt(shm) == -1)
        {
            perror("P1 shmdt failed");
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl failed");
        }
        else
        {
            printf("[P1] Shared memory successfully detached and removed.\n");
        }
    }

    return 0;
}