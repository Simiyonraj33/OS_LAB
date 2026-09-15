#include <stdio.h>
#include <stdlib.h>

/* Function declarations */
int detectSafety(int m, int n, int avail[n],
                 int max[m][n], int alloc[m][n]);

int processResourceReq(int m, int n, int p, int req[n],
                       int avail[n], int max[m][n], int alloc[m][n]);


int main()
{
    int m, n, i, j;
    int p;

    /* Number of processes */
    printf("Enter number of processes: ");
    scanf("%d", &m);

    /* Number of resources */
    printf("Enter number of resources: ");
    scanf("%d", &n);

    int alloc[m][n];
    int max[m][n];
    int avail[n];
    int req[n];


    /* ---------------- Allocation Matrix ---------------- */

    printf("\nEnter Allocation Matrix:\n");

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }


    /* ---------------- Max Matrix ---------------- */

    printf("\nEnter Max Matrix:\n");

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }


    /* ---------------- Available Resources ---------------- */

    printf("\nEnter Available Resources:\n");

    for(j = 0; j < n; j++)
    {
        scanf("%d", &avail[j]);
    }


    /* ---------------- Need Matrix ---------------- */

    printf("\nNeed Matrix:\n");

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            printf("%d ", max[i][j] - alloc[i][j]);
        }

        printf("\n");
    }


    /* ---------------- Initial Safety Check ---------------- */

    printf("\nChecking Initial System Safety...\n");

    if(detectSafety(m, n, avail, max, alloc))
    {
        printf("System is SAFE.\n");
    }
    else
    {
        printf("System is NOT SAFE.\n");
    }


    /* ---------------- Resource Request ---------------- */

    printf("\nEnter process number making request: ");
    scanf("%d", &p);


    /* Check whether process ID is valid */

    if(p < 0 || p >= m)
    {
        printf("Invalid process number.\n");
        return 0;
    }


    /* Get resource request for only this process */

    printf("Enter Resource Request for P%d:\n", p);

    for(j = 0; j < n; j++)
    {
        scanf("%d", &req[j]);
    }


    /* Process the request */

    if(processResourceReq(m, n, p, req,
                          avail, max, alloc))
    {
        printf("Request can be GRANTED.\n");
    }
    else
    {
        printf("Request cannot be GRANTED.\n");
    }


    return 0;
}


/* =========================================================
   Function to detect Safe State
   ========================================================= */

int detectSafety(int m, int n, int avail[n],
                 int max[m][n], int alloc[m][n])
{
    int work[n];
    int finish[m];
    int safe[m];

    int i, j;
    int count = 0;
    int found;


    /* Work = Available */

    for(j = 0; j < n; j++)
    {
        work[j] = avail[j];
    }


    /* Initially all processes are unfinished */

    for(i = 0; i < m; i++)
    {
        finish[i] = 0;
    }


    /* Find Safe Sequence */

    while(count < m)
    {
        found = 0;

        for(i = 0; i < m; i++)
        {
            if(finish[i] == 0)
            {
                int possible = 1;


                /* Check Need <= Work */

                for(j = 0; j < n; j++)
                {
                    if(max[i][j] - alloc[i][j] > work[j])
                    {
                        possible = 0;
                        break;
                    }
                }


                /* If process can execute */

                if(possible)
                {
                    /* Work = Work + Allocation */

                    for(j = 0; j < n; j++)
                    {
                        work[j] = work[j] + alloc[i][j];
                    }


                    /* Add process to safe sequence */

                    safe[count] = i;

                    finish[i] = 1;

                    count++;

                    found = 1;
                }
            }
        }


        /* No process can execute */

        if(found == 0)
        {
            break;
        }
    }


    /* ---------------- Print Safe Sequence ---------------- */

    if(count == m)
    {
        printf("Safe Sequence: ");

        for(i = 0; i < m; i++)
        {
            printf("P%d", safe[i]);

            if(i != m - 1)
            {
                printf(" -> ");
            }
        }

        printf("\n");

        return 1;
    }


    return 0;
}


/* =========================================================
   Function to Process Resource Request
   ========================================================= */

int processResourceReq(int m, int n, int p, int req[n],
                       int avail[n], int max[m][n],
                       int alloc[m][n])
{
    int j;


    /* ---------------- Check Request <= Need ---------------- */

    for(j = 0; j < n; j++)
    {
        if(req[j] > max[p][j] - alloc[p][j])
        {
            printf("\nError: Request exceeds maximum need.\n");
            printf("System is NOT SAFE for this request.\n");

            return 0;
        }
    }


    /* ---------------- Check Request <= Available ---------------- */

    for(j = 0; j < n; j++)
    {
        if(req[j] > avail[j])
        {
            printf("\nResources are not available.\n");
            printf("System is NOT SAFE for this request.\n");

            return 0;
        }
    }


    /* ---------------- Temporarily Allocate ---------------- */

    for(j = 0; j < n; j++)
    {
        avail[j] = avail[j] - req[j];

        alloc[p][j] = alloc[p][j] + req[j];
    }


    printf("\nChecking Safety After Granting Request...\n");


    /* ---------------- Check Safety ---------------- */

    if(detectSafety(m, n, avail, max, alloc))
    {
        printf("System is SAFE after granting the request.\n");

        return 1;
    }


    /* ---------------- Rollback ---------------- */

    printf("System is NOT SAFE after granting the request.\n");

    printf("Rolling back the request...\n");


    for(j = 0; j < n; j++)
    {
        avail[j] = avail[j] + req[j];

        alloc[p][j] = alloc[p][j] - req[j];
    }


    return 0;
}