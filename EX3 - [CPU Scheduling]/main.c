#include <stdio.h>
#include "header.h"

int main() {

    int n, choice;
    int run_again = 1;

    float at[10], bt[10];
    float ct[10], tat[10], wt[10];

    int priority[10];
    float tq;


    /* Get number of processes */

    printf("Enter the number of processes: ");
    scanf("%d", &n);


    /* Get Arrival Time and Burst Time */

    for(int i = 0; i < n; i++) {

        printf("Enter Arrival Time and Burst Time of P%d: ",
               i + 1);

        scanf("%f %f", &at[i], &bt[i]);
    }


    /* Main menu */

    while(run_again == 1) {

        printf("\n==============================");
        printf("\n CPU SCHEDULING ALGORITHMS");
        printf("\n==============================");

        printf("\n1. FCFS");
        printf("\n2. SJF (Non-Preemptive)");
        printf("\n3. SRTF (Preemptive)");
        printf("\n4. Round Robin");
        printf("\n5. Priority (Non-Preemptive)");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);


        /* FCFS */

        if(choice == 1) {

            calculate_fcfs(n, at, bt, ct, tat, wt);

            printf("\n--- FCFS ---\n");

            display_table(n, at, bt, ct, tat, wt);
        }


        /* SJF */

        else if(choice == 2) {

            calculate_sjf(n, at, bt, ct, tat, wt);

            printf("\n--- SJF ---\n");

            display_table(n, at, bt, ct, tat, wt);
        }


        /* SRTF */

        else if(choice == 3) {

            calculate_srtf(n, at, bt, ct, tat, wt);

            printf("\n--- SRTF ---\n");

            display_table(n, at, bt, ct, tat, wt);
        }


        /* Round Robin */

        else if(choice == 4) {

            printf("Enter Time Quantum: ");
            scanf("%f", &tq);

            calculate_rr(n, at, bt, ct, tat, wt, tq);

            printf("\n--- Round Robin ---\n");

            display_table(n, at, bt, ct, tat, wt);
        }


        /* Priority */

        else if(choice == 5) {

            printf("\nEnter Priority for each process");
            printf("\n(Smaller number = Higher Priority)\n");

            for(int i = 0; i < n; i++) {

                printf("Priority of P%d: ", i + 1);

                scanf("%d", &priority[i]);
            }

            calculate_priority(n, at, bt, priority,
                               ct, tat, wt);

            printf("\n--- Priority Scheduling ---\n");

            display_table(n, at, bt, ct, tat, wt);
        }


        /* Invalid choice */

        else {

            printf("\nInvalid Choice!");
        }


        /* Continue */

        printf("\nDo you want to execute another algorithm?");
        printf("\nPress 1 for Yes, 0 for No: ");

        scanf("%d", &run_again);
    }


    printf("\nProgram exited successfully. Goodbye!\n");

    return 0;
}