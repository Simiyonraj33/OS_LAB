#include <stdio.h>
#include "header.h"

/* ---------------- FCFS ---------------- */

void calculate_fcfs(int n, float at[], float bt[],
                    float ct[], float tat[], float wt[]) {

    float time = 0;

    for(int i = 0; i < n; i++) {

        if(time < at[i])
            time = at[i];

        time = time + bt[i];

        ct[i] = time;
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }
}


/* ---------------- SJF Non-Preemptive ---------------- */

void calculate_sjf(int n, float at[], float bt[],
                   float ct[], float tat[], float wt[]) {

    int completed[10] = {0};
    int count = 0;
    int index;
    float time = 0;
    float min;

    while(count < n) {

        min = 9999;
        index = -1;

        for(int i = 0; i < n; i++) {

            if(at[i] <= time && completed[i] == 0) {

                if(bt[i] < min) {
                    min = bt[i];
                    index = i;
                }
            }
        }

        if(index == -1) {
            time++;
        }
        else {

            time = time + bt[index];

            ct[index] = time;
            tat[index] = ct[index] - at[index];
            wt[index] = tat[index] - bt[index];

            completed[index] = 1;
            count++;
        }
    }
}


/* ---------------- SRTF ---------------- */

void calculate_srtf(int n, float at[], float bt[],
                    float ct[], float tat[], float wt[]) {

    float rt[10];
    float time = 0;
    float min;
    int index;
    int completed = 0;

    for(int i = 0; i < n; i++)
        rt[i] = bt[i];

    while(completed < n) {

        min = 9999;
        index = -1;

        for(int i = 0; i < n; i++) {

            if(at[i] <= time && rt[i] > 0) {

                if(rt[i] < min) {
                    min = rt[i];
                    index = i;
                }
            }
        }

        if(index == -1) {
            time++;
        }
        else {

            rt[index]--;
            time++;

            if(rt[index] == 0) {

                ct[index] = time;
                tat[index] = ct[index] - at[index];
                wt[index] = tat[index] - bt[index];

                completed++;
            }
        }
    }
}


/* ---------------- Round Robin ---------------- */

void calculate_rr(int n, float at[], float bt[],
                  float ct[], float tat[], float wt[],
                  float tq) {

    float rt[10];
    float time = 0;
    int completed = 0;

    for(int i = 0; i < n; i++)
        rt[i] = bt[i];

    while(completed < n) {

        int found = 0;

        for(int i = 0; i < n; i++) {

            if(at[i] <= time && rt[i] > 0) {

                found = 1;

                if(rt[i] > tq) {

                    time = time + tq;
                    rt[i] = rt[i] - tq;
                }
                else {

                    time = time + rt[i];
                    rt[i] = 0;

                    ct[i] = time;
                    tat[i] = ct[i] - at[i];
                    wt[i] = tat[i] - bt[i];

                    completed++;
                }
            }
        }

        if(found == 0)
            time++;
    }
}


/* ---------------- Priority Scheduling ---------------- */

void calculate_priority(int n, float at[], float bt[],
                        int priority[], float ct[],
                        float tat[], float wt[]) {

    int completed[10] = {0};
    int count = 0;
    int index;
    float time = 0;
    int high;

    while(count < n) {

        index = -1;
        high = 9999;

        for(int i = 0; i < n; i++) {

            if(at[i] <= time && completed[i] == 0) {

                if(priority[i] < high) {

                    high = priority[i];
                    index = i;
                }
            }
        }

        if(index == -1) {
            time++;
        }
        else {

            time = time + bt[index];

            ct[index] = time;
            tat[index] = ct[index] - at[index];
            wt[index] = tat[index] - bt[index];

            completed[index] = 1;
            count++;
        }
    }
}


/* ---------------- Display Table ---------------- */

void display_table(int n, float at[], float bt[],
                   float ct[], float tat[], float wt[]) {

    float avgWT = 0;
    float avgTAT = 0;

    printf("\n---------------------------------------------------------");
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT");
    printf("\n---------------------------------------------------------");

    for(int i = 0; i < n; i++) {

        printf("\nP%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f",
               i + 1,
               at[i],
               bt[i],
               ct[i],
               tat[i],
               wt[i]);

        avgTAT = avgTAT + tat[i];
        avgWT = avgWT + wt[i];
    }

    avgTAT = avgTAT / n;
    avgWT = avgWT / n;

    printf("\n---------------------------------------------------------");

    printf("\nAverage Turnaround Time = %.2f", avgTAT);
    printf("\nAverage Waiting Time = %.2f\n", avgWT);
}