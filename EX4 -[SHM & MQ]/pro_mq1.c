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

    // Create message queue
    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget failed");
        return 1;
    }

    data.msg_type = 1;

    printf("Producer: Enter message: ");

    fgets(data.message, 200, stdin);

    // Remove newline
    data.message[strcspn(data.message, "\n")] = '\0';

    // Send message
    if (msgsnd(msgid, &data, sizeof(data.message), 0) == -1)
    {
        perror("msgsnd failed");
        return 1;
    }

    printf("Producer: Message sent = %s\n", data.message);

    return 0;
}