#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX_SUBJECTS 6

// Structure holding student details
struct student_data {
    char name[50];
    int roll_no;
    int num_subjects;
    int marks[MAX_SUBJECTS];
};

// Message structure wrapper
struct msg_buffer {
    long msg_type;
    struct student_data student;
};

// Function to calculate letter grade based on marks
char get_grade(float percentage) {
    if (percentage >= 90) return 'S';
    if (percentage >= 80) return 'A';
    if (percentage >= 70) return 'B';
    if (percentage >= 60) return 'C';
    if (percentage >= 50) return 'D';
    return 'F';
}

int main() {
    key_t key = 5678;

    // Create Message Queue
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) {
        // ================= PRODUCER (PARENT) =================
        struct msg_buffer message;
        message.msg_type = 1;

        printf("=== PRODUCER: ENTER STUDENT DETAILS ===\n");

        printf("Student Name: ");
        fgets(message.student.name, sizeof(message.student.name), stdin);
        message.student.name[strcspn(message.student.name, "\n")] = '\0';

        printf("Roll Number: ");
        scanf("%d", &message.student.roll_no);

        printf("Enter total number of subjects: ");
        scanf("%d", &message.student.num_subjects);

        for (int i = 0; i < message.student.num_subjects; i++) {
            printf("Enter marks for Subject %d (out of 100): ", i + 1);
            scanf("%d", &message.student.marks[i]);
        }

        // Send struct via message queue
        if (msgsnd(msgid, &message, sizeof(struct student_data), 0) < 0) {
            perror("msgsnd failed");
            exit(1);
        }

        printf("\n[Producer] Student details sent to Consumer via Message Queue.\n");

        if (wait(NULL) < 0) {
            perror("wait failed");
            exit(1);
        }

        // Cleanup queue
        if (msgctl(msgid, IPC_RMID, NULL) < 0) {
            perror("msgctl failed");
            exit(1);
        }

    } else {
        // ================= CONSUMER (CHILD) =================
        struct msg_buffer message;

        // Receive message
        if (msgrcv(msgid, &message, sizeof(struct student_data), 1, 0) < 0) {
            perror("msgrcv failed");
            exit(1);
        }

        struct student_data s = message.student;
        int total_marks = 0;

        for (int i = 0; i < s.num_subjects; i++) {
            total_marks += s.marks[i];
        }

        float percentage = (float)total_marks / s.num_subjects;
        char final_grade = get_grade(percentage);

        // Print Grade Sheet
        printf("\n=========================================\n");
        printf("           ACADEMIC GRADE SHEET          \n");
        printf("=========================================\n");
        printf(" Name        : %s\n", s.name);
        printf(" Roll No     : %d\n", s.roll_no);
        printf("-----------------------------------------\n");

        for (int i = 0; i < s.num_subjects; i++) {
            printf(" Subject %-2d  : %d / 100\n", i + 1, s.marks[i]);
        }

        printf("-----------------------------------------\n");
        printf(" Total Marks : %d / %d\n", total_marks, s.num_subjects * 100);
        printf(" Percentage  : %.2f%%\n", percentage);
        printf(" Final Grade : %c\n", final_grade);
        printf("=========================================\n");

        exit(0);
    }

    return 0;
}