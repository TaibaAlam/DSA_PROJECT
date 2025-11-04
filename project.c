#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Job
{
    int jobId;
    int pages;
    int priority;
} Job;

typedef struct MinHeap
{
    Job **jobs;
    int size;  
    int capacity; 
} MinHeap;


Job *createJob(int jobId, int pages, int priority)
{
    Job *newjob = (Job *)malloc(sizeof(Job));
    newjob->jobId = jobId;
    newjob->pages = pages;
    newjob->priority = priority; // Priority is now from user input
    return newjob;
}


MinHeap *createMinHeap(int capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->jobs = (Job **)malloc(capacity * sizeof(Job *));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}


void swapJobs(Job **a, Job **b)
{
    Job *temp = *a;
    *a = *b;
    *b = temp;
}


void heapifyUp(MinHeap *heap, int index)
{
    int parent = (index - 1) / 2;
    while (index > 0 && heap->jobs[index]->priority < heap->jobs[parent]->priority)
    {
        swapJobs(&heap->jobs[index], &heap->jobs[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}


void heapifyDown(MinHeap *heap, int index)
{
    int smallest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < heap->size && heap->jobs[leftChild]->priority < heap->jobs[smallest]->priority)
    {
        smallest = leftChild;
    }

    if (rightChild < heap->size && heap->jobs[rightChild]->priority < heap->jobs[smallest]->priority)
    {
        smallest = rightChild;
    }

    if (smallest != index)
    {
        swapJobs(&heap->jobs[index], &heap->jobs[smallest]);
        heapifyDown(heap, smallest);
    }
}


void insertJob(MinHeap *heap, int jobId, int pages, int priority)
{
    if (heap->size == heap->capacity)
    {
        printf("Printer queue is full. Cannot add job.\n");
        return;
    }

    Job *newJob = createJob(jobId, pages, priority);
    heap->jobs[heap->size] = newJob;
    int currentIndex = heap->size;
    heap->size++;

    heapifyUp(heap, currentIndex);

    printf("Job %d with %d pages and priority %d added to queue.\n", jobId, pages, priority);
}


void processNextJob(MinHeap *heap)
{
    if (heap->size == 0)
    {
        printf("No jobs to process.\n");
        return;
    }

    Job *jobToProcess = heap->jobs[0];
    heap->jobs[0] = heap->jobs[heap->size - 1];
    heap->size--;

    heapifyDown(heap, 0);
    printf("\nProcessing Job ID: %d (Pages: %d, Priority: %d)\n",jobToProcess->jobId, jobToProcess->pages, jobToProcess->priority);

    for (int i = 1; i <= jobToProcess->pages; i++)
    {
        printf("    Page %d printed.\n", i);
        sleep(1);
    }
    printf("Job %d completed.\n", jobToProcess->jobId);
    free(jobToProcess);
}


void displayHeap(MinHeap *heap)
{
    if (heap->size == 0)
    {
        printf("No jobs in queue.\n");
        return;
    }

    printf("\nCurrent Printer Queue:\n");
    printf("----------------------------------\n");
    printf("Job ID\t|Pages\t|Priority\n");
    
    for (int i = 0; i < heap->size; i++)
    {
        printf(" %d\t| %d\t| %d\n",
               heap->jobs[i]->jobId, heap->jobs[i]->pages, heap->jobs[i]->priority);
    }
}


int main()
{
    MinHeap *heap = createMinHeap(100);
    int choice, jobId, pages, priority;

    printf("Printer Job Scheduler using Min-Heap:\n");

    while (1)
    {
        printf("\n1. Add Print Job");
        printf("\n2. Process Next Job (Highest Priority)");
        printf("\n3. Display All Jobs (Heap View)");
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
            printf("Enter Priority (lower number = higher priority): ");
            scanf("%d", &priority);
            insertJob(heap, jobId, pages, priority);
            break;

        case 2:
            processNextJob(heap);
            break;

        case 3:
            displayHeap(heap);
            break;

        case 4:
            printf("\nExiting Printer Job Scheduler...\n");
            while (heap->size > 0)
            {
                processNextJob(heap);
            }
            free(heap->jobs);
            free(heap);
            exit(0);

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
