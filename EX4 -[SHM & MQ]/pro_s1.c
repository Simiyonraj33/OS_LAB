#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct Data
{
    char str[100];
    int ready;
    int done;
};

int main()
{
    key_t key;
    int shmid;
    struct Data *data;

    // Generate key
    key = ftok(".", 'C');

    if (key == -1)
    {
        perror("ftok failed");
        return 1;
    }

    // Create shared memory
    shmid = shmget(key, sizeof(struct Data), IPC_CREAT | 0666);

    if (shmid == -1)
    {
        perror("shmget failed");
        return 1;
    }

    // Attach shared memory
    data = (struct Data *)shmat(shmid, NULL, 0);

    if (data == (void *)-1)
    {
        perror("shmat failed");
        return 1;
    }

    // Initialize synchronization variables
    data->ready = 0;
    data->done = 0;

    printf("Producer: Enter String: ");
    fgets(data->str, 100, stdin);

    // Remove newline
    data->str[strcspn(data->str, "\n")] = '\0';

    printf("Producer: String stored in shared memory.\n");

    // Tell consumer that string is ready
    data->ready = 1;

    // Wait for consumer to finish processing
    while (data->done == 0)
    {
    }

    printf("Producer: Reversed string received = %s\n", data->str);

    // Detach shared memory
    if (shmdt(data) == -1)
    {
        perror("shmdt failed");
        return 1;
    }

    // Remove shared memory
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl failed");
        return 1;
    }

    return 0;
}