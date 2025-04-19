#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define QUANTUM 3
#define TIME_LIMITE 100
#define NUMBER_PROCESS 20

int time = 1;

void scheduler(Queue *QueueDestiny, Queue *Ready, Status status);
Process *findByTime(Queue *Queue);

/*
    Do: search for element by time 
    return: element address if exists else null 
*/
Process *findByTime(Queue *Queue)
{
    int size = Queue->size;
    if( !isEmpty(Queue) )
        return NULL;

    for( int i = size; i > 0; i-- )
    {
        Process *auxProcess = getQueueNodeAt(Queue, i);

        if( auxProcess->timeEnqueue == time )
            return auxProcess; // find element
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

Process *findByPID(Queue *Queue, int pid)
{    
    int size = Queue->size;
    if( !isEmpty(Queue) )
        return NULL;
    for( int i = size; i > 0; i-- )
    {
        Process *auxProcess = getQueueNodeAt(Queue, i);
    
        if( auxProcess->pid == pid )
            return auxProcess; // find element
    }
        return NULL; // no element with time out
}

/*
*/
void printProcess(Process *processor, Queue *New, Queue *Blocked, Queue *Ready, Queue *Exit)
{
    // Cabeçalho da tabela
    printf("%-10s", "time inst");
    for (int i = 1; i <= NUMBER_PROCESS; i++) {
        printf("%-15s", "proc");
    }
    printf("\n");

    // Dados da tabela
    for (int i = 1; i <= NUMBER_PROCESS; i++) {
        printf("%-10d", time); // Coluna de tempo
        if (processor != NULL && processor->pid == i) {
            printf("%-15s", "RUNNING");
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
        printf("\n");
    }

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
        printf("node: %d ", process->pid);
    }

    printf("\n");
    removeNodeByData(NewQueue, process1);
    */

    while( time <= TIME_LIMITE )
    {   
        printProcess(processor, NewQueue, BloquedQueue, ReadyQueue, ExitQueue);

        time ++;
    }   

    return 0;
}