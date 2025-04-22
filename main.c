#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define QUANTUM 3
#define TIME_LIMITE 100
#define NUMBER_PROCESS 5

int time = 0;

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
    return NULL; // no element with time-out
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
        return NULL; // no element with time-out
}

/*
    scheduler    
*/
void scheduler(Queue *QueueDestiny, Queue *Ready, Status status)
{
    Process *process = findByTime(QueueDestiny); 
    if( process == NULL ) //stop recursive call
        return;
    if( process->status == NEW )
    {
        process = (Process *)dequeue(QueueDestiny);
    }
    else if ( process->status == BLOCKED )
    {
        process = removeNodeByData(QueueDestiny, process);  // remove in whatover position
    }
    else if ( process->status == EXIT )
    {
        Process *aux = dequeue(QueueDestiny);
        killProcess(aux); // dump the process
        scheduler(QueueDestiny, Ready, status);  //repeate while exists any process
        return;
    }
    process->status = status;
    enqueue(Ready, process);

    scheduler(QueueDestiny, Ready, status);
}

/*
    dispatcher
*/
void dispatcher(Process *processor, Queue *QueueDestiny, Status status)
{
    processor->status = status;
    processor->timeCpu = 0;
    
    if( status == BLOCKED )
    {
        int pc = processor->programCounter;
        int timeEnqueue = (processor->context[pc] * -1) + time;

        processor->timeEnqueue = timeEnqueue;
    }
    else if( status == EXIT )
    {
        processor->timeEnqueue = time + 1;
    }

    enqueue(QueueDestiny, processor);
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

    /*
    int programs[LINE][COLUN] = { {204, 202, 3},
    {1, 204, 1},
    {2, 201, 5},
    {0, 0, 10},
    {0, 0, 0} };*/

    int programs[LINE][COLUN] = { 
        { 203,    4,     2 },
        {   1,   -3,   202 },
        {  -2,  102,     5 },
        {   1,    0,    10 },
        {   0,    0,     0 }};

    Process *process;
    Process *processor = NULL;
  
    Queue *NewQueue = createQueue();
    Queue *BloquedQueue  = createQueue();
    Queue *ReadyQueue = createQueue();
    Queue *ExitQueue = createQueue();

    process = forks(programs[0], &globalPid, time);
    enqueue(NewQueue, process);

    // Cabeçalho da tabela
    printf("%-10s", "time inst");
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        printf("%-15s", "proc");
    }
    printf("\n");

    while( time < TIME_LIMITE )
    {   
        time ++;
        printProcess(processor, NewQueue, BloquedQueue, ReadyQueue, ExitQueue);
        
        scheduler(BloquedQueue, ReadyQueue, READY);
        scheduler(NewQueue, ReadyQueue, READY);
        scheduler(ExitQueue, ReadyQueue, EXIT);

        if( processor == NULL )
        {
            Process *auxProcess = (Process *)dequeue(ReadyQueue);
            
            if ( auxProcess != NULL )
            {
                auxProcess->status = RUNNING;
                processor = auxProcess;
                processor->timeCpu = 1;
            }
        }
        else
        {
            //time-out
            if( processor->timeCpu == QUANTUM )
            {
                dispatcher(processor, ReadyQueue, READY);
                processor = NULL;
            }
            else
            {
                if( processor->programCounter > 2 )
                {    
                    dispatcher(processor, ExitQueue, EXIT);
                    processor = NULL;
                    continue;
                }
                else
                {
                    int index = processor->programCounter; 
                    int pc = processor->context[index];

                    if( pc > 100 && pc < 200 )  // JUMP
                    {
                        int jump = pc % 100;
                        processor->programCounter -= jump;
                    }
                    else if ( pc > 200 && pc < 300 ) //EXEC
                    {
                        int index = pc % 100;
                        Process *forkProcess = forks(programs[index], &globalPid, time);
                        enqueue(NewQueue, forkProcess);
                    }
                    else if ( pc < 0 ) //IO
                    {
                        dispatcher(processor, BloquedQueue, BLOCKED);
                        processor = NULL;
                    }
                    else if( pc == 0 ) //HALT
                    {
                        dispatcher(processor, ExitQueue, EXIT);
                        processor = NULL;
                    }
                }
                if( processor != NULL )
                {
                    processor->programCounter += 1;
                    processor->timeCpu += 1;
                }
            }
                
        }
    }   

    return 0;
}