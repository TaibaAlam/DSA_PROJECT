# DSA_PROJECT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Job
{
    int jobId;
    int pages;
    int priority;
    struct Job *next;
} Job;

Job *createJob(int jobId, int pages)
{
    Job *newjob = (Job *)malloc(sizeof(Job));
    newjob->jobId = jobId;
    newjob->pages = pages;
    newjob->priority = pages;
    newjob->next = NULL;
    return newjob;
}

void enqueue(Job **front, int jobId, int pages)
{
    Job *newJob = createJob(jobId, pages);

    if ((*front) == NULL || newJob->priority < (*front)->priority)
    {
        newJob->next = (*front);
        (*front) = newJob;
    }
    else
    {
        Job *temp = (*front);
        while (temp->next != NULL && temp->next->priority <= newJob->priority)
        {
            temp = temp->next;
        }
        newJob->next = temp->next;
        temp->next = newJob;
    }

    printf("Job %d with %d pages added to queue.\n", jobId, pages);
    // return front;
}

void dequeue(Job **front)
{
    if ((*front) == NULL)
    {
        printf("⚠  No jobs to process.\n");
        return NULL;
    }

    Job *temp = (*front);
    (*front) = (*front)->next;

    printf("\nProcessing Job ID: %d (%d pages)\n", temp->jobId, temp->pages);
    for (int i = 1; i <= temp->pages; i++)
    {
        printf("   Page %d printed.\n", i);
        sleep(1);
    }
    printf("Job %d completed.\n", temp->jobId);
    free(temp);

    // return front;
}

void displayQueue(Job *front)
{
    if (front == NULL)
    {
        printf("No jobs in queue.\n");
        return;
    }

    printf("\nCurrent Printer Queue (Sorted by Fewest Pages First):\n");
    printf("----------------------------------\n");
    printf("Job ID\t|Pages\t|Priority\n");

    Job *temp = front;
    while (temp != NULL)
    {
        printf(" %d\t| %d\t| %d\n", temp->jobId, temp->pages, temp->priority);
        temp = temp->next;
    }
}

int main()
{
    Job *front = NULL;
    int choice, jobId, pages;

    printf("Printer Job Scheduler using Priority Queue:\n");

    while (1)
    {
        printf("\n1. Add Print Job");
        printf("\n2. Process Next Job");
        printf("\n3. Display All Jobs");
        printf("\n4. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter Job ID: ");
            scanf("%d", &jobId);
            printf("Enter Number of Pages: ");
            scanf("%d", &pages);
            enqueue(&front, jobId, pages);
            break;

        case 2:
            dequeue(&front);
            break;

        case 3:
            displayQueue(front);
            break;

        case 4:
            printf("\nExiting Printer Job Scheduler...\n");
            while (front != NULL)
                dequeue(&front);
            exit(0);

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
