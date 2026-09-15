#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Message structure
struct msg_buffer
{
    long msg_type;
    char message[200];
};

int main()
{
    key_t key = 1234;
    int msgid;

    struct msg_buffer data;

    // Access message queue
    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget failed");
        return 1;
    }

    printf("Consumer: Waiting for message...\n");

    // Receive message
    if (msgrcv(msgid, &data, sizeof(data.message), 1, 0) == -1)
    {
        perror("msgrcv failed");
        return 1;
    }

    printf("Consumer: Message received = %s\n", data.message);

    // Count words
    int words = 0;

    for (int i = 0; data.message[i] != '\0'; i++)
    {
        if (data.message[i] != ' ' &&
            (i == 0 || data.message[i - 1] == ' '))
        {
            words++;
        }
    }

    printf("Consumer: Number of words = %d\n", words);

    // Remove message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl failed");
        return 1;
    }

    return 0;
}