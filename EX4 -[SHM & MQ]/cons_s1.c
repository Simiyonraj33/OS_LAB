#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

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

    // Access shared memory
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

    printf("Consumer: Waiting for string...\n");

    // Wait until producer enters the string
    while (data->ready == 0)
    {
    }

    printf("Consumer: String received = %s\n", data->str);

    int n = strlen(data->str);

    // Reverse the string
    for (int i = 0; i < n / 2; i++)
    {
        char temp = data->str[i];
        data->str[i] = data->str[n - i - 1];
        data->str[n - i - 1] = temp;
    }

    printf("Consumer: Reverse String = %s\n", data->str);

    // Tell producer that processing is complete
    data->done = 1;

    // Detach shared memory
    if (shmdt(data) == -1)
    {
        perror("shmdt failed");
        return 1;
    }

    return 0;
}