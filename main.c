#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define QUANTUM 3
#define TIME_LIMITE 100
#define NUMBER_PROCESS 5

int time = 1;

void scheduler(Queue *QueueDestiny, Queue *Ready, Status status);
Process *findByTime(Queue *Queue);
Process *findByPID(Queue *Queue, int pid);

/*
    Do: search for element by time 
    return: element address if exists else null 
*/
Process *findByTime(Queue *Queue)
{
    if( isEmpty(Queue) )
        return NULL;
    for( QueueNode *node = Queue->front; node != NULL; node = node->next )
    {
        Process *process = (Process *)node->data;
        if( process->timeEnqueue == time )
            return process; // find element
    }
    return NULL; // no element with time out
}

/*
    
*/
Process *findByPID(Queue *Queue, int pid)
{    
    if( isEmpty(Queue) )
        return NULL;
    for( QueueNode *node = Queue->front; node != NULL; node = node->next )
    {
        Process *process = (Process *)node->data;
        if( process->pid == pid )
            return process; // find element
    }
        return NULL; // no element with time out
}

/*
    scheduler    
*/
void scheduler(Queue *QueueDestiny, Queue *Ready, Status status)
{
    Process *process = findByTime(QueueDestiny);
    if( process == NULL )
        return;
    if( process->status != BLOCKED )
    {
        process = (Process *)dequeue(QueueDestiny);
    }
    else
    {
        process = removeNodeByData(QueueDestiny, process); 
    }
    process->status = status;
    //process->timeEnqueue = 1;
    enqueue(Ready, process);

    scheduler(QueueDestiny, Ready, status);
}

/*
    dispatcher
*/
void dispatcher(Process *processor, Queue *QueueDestiny, Queue *Ready, Status status)
{
    processor->status = status;
    processor->timeCpu = 1;
    //processor->timeEnqueue = 1;
    //processor->programCounter += 1;
    enqueue(QueueDestiny, processor);

    Process *process = (Process *)dequeue(Ready);
    process->status = RUNNING;
    processor = process;
}   

/*
*/
void printProcess(Process *processor, Queue *New, Queue *Blocked, Queue *Ready, Queue *Exit)
{
    printf("%-10d", time); 
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        if (processor != NULL && processor->pid == i) {
            printf("%-15s", getStatus(processor->status));
        } else {
            Process *process = findByPID(Blocked, i);
            if (process != NULL) {
                printf("%-15s", getStatus(process->status));
            } else if ((process = findByPID(Ready, i)) != NULL) {
                printf("%-15s", getStatus(process->status));
            } else if ((process = findByPID(New, i)) != NULL) {
                printf("%-15s", getStatus(process->status));
            } else if ((process = findByPID(Exit, i)) != NULL) {
                printf("%-15s", getStatus(process->status));
            } 
        }
    }
    printf("\n");
}

/*
    main
*/
int main()
{
    int globalPid = 1, nProcessKill = 0;

    int programs[LINE][COLUN] = { {3, 4, 2},
    {1, 2, 1},
    {2, 4, 5},
    {0, 0, 10},
    {0, 0, 0} };

    Process *process;
    Process *processor = NULL;
  
    Queue *NewQueue = createQueue();
    Queue *BloquedQueue  = createQueue();
    Queue *ReadyQueue = createQueue();
    Queue *ExitQueue = createQueue();

    process = forks(programs[0], &globalPid);
    Process *process1 = forks(programs[1], &globalPid);
    Process *process2 = forks(programs[2], &globalPid);
    Process *process3 = forks(programs[3], &globalPid);

    
    enqueue(NewQueue, process);
    enqueue(NewQueue, process1);
    enqueue(NewQueue, process2);
    enqueue(NewQueue, process3);

    /*
    for( QueueNode *node = NewQueue->front; node != NULL; node = node->next )
    {
        Process *process = (Process *)node->data;
        printf("node: %s ", getStatus(process->status));
    }
    
    printf("\n");
    removeNodeByData(NewQueue, process1);
    */

    // Cabeçalho da tabela
    printf("%-10s", "time inst");
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        printf("%-15s", "proc");
    }
    printf("\n");

    while( time <= TIME_LIMITE )
    {   
        printProcess(processor, NewQueue, BloquedQueue, ReadyQueue, ExitQueue);

        time ++;
    }   

    return 0;
}