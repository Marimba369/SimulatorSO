#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define QUANTUM 3
#define TIME_LIMITE 100

/*
    Do: search for element in queue
    return: element address if exists else null 
*/
Process *findByTime(Queue *Queue, int time)
{
    int size = Queue->size; 
    if( size == 0 )
        return NULL; // no element in queue

    for( int i = 0, j = 0; i < size; i++ )
    {
        Process *auxProcess = getQueueNodeAt(Queue, i);

        if( auxProcess->timeEnqueue == time )
            return auxProcess; // find element
    }
    return NULL; // no element with time out
}

/*
    main
*/
int main()
{
    int counter = 0, globalPid = 1, nProcessKill = 0;

    int programs[LINE][COLUN] = { {3, 4, 2},
    {1, 2, 1},
    {2, 4, 5},
    {0, 0, 10},
    {0, 0, 0} };

    Process *process;
    Process processor;
  
    Queue *NewQueue = createQueue();
    Queue *BloquedQueue  = createQueue();
    Queue *ReadyQueue = createQueue();

    process = forks(programs[0], &globalPid);

    enqueue(NewQueue, process);
    Process *aux = (Process *)dequeue(NewQueue);

    while( counter < TIME_LIMITE )
    {   
        

        counter ++;
    }   

    return 0;
}