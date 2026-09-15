#include "shared.h"

int main()
{
    int shmid, semid, i;
    key_t key;
    struct SharedData *data;

    key = ftok(SHM_KEY_PATH, PROJ_ID);

    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    shmid = shmget(key, sizeof(struct SharedData), 0666);

    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }

    data = (struct SharedData *)shmat(shmid, NULL, 0);

    if (data == (struct SharedData *)-1)
    {
        perror("shmat");
        return 1;
    }

    semid = semget(key, 3, 0666);

    if (semid == -1)
    {
        perror("semget");
        return 1;
    }

    for (i = 0; i < 10; i++)
    {
        int no;

        wait_sem(semid, FULL);
        wait_sem(semid, MUTEX);

        no = data->buff[data->out];

        printf("CONSUMER CONSUMED: %d\n", no);

        data->out = (data->out + 1) % SIZE;

        signal_sem(semid, MUTEX);
        signal_sem(semid, EMPTY);
    }

    printf("Consumer complete.\n");

    shmdt(data);

    return 0;
}