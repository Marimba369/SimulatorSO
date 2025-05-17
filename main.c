#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define QUANTUM 3
#define TIME_LIMITE 100
#define NUMBER_PROCESS 20
#define MAX_PROCESS 20

int time = 0;

void scheduler(Queue *QueueDestiny, Queue *Ready, Status status);
Process *findByTime(Queue *Queue);
Process *findByPID(Queue *Queue, int pid);

/*
    Do: search for exlement by time 
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
        process = removeNodeByData(QueueDestiny, process);  // remove in whatever position
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
*/

void printProcess(FILE *file, Process *processor, Queue *New, Queue *Blocked, Queue *Ready, Queue *Exit)
{
    fprintf(file, "%-10d", time); 
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        if (processor != NULL && processor->pid == i) {
            fprintf(file, "%-15s", getStatus(processor->status));
        } else {
            Process *process = findByPID(Blocked, i);
            if (process != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else if ((process = findByPID(Ready, i)) != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else if ((process = findByPID(New, i)) != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else if ((process = findByPID(Exit, i)) != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } 
        }
    }
    fprintf(file, "\n");
}



/*
    main
*/
int main()
{
    int globalPid = 1;

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return 1;
    }

    /*
    int programs[LINE][COLUN] = { {204, 202, 3},
    {1, 204, 1},
    {2, 201, 5},
    {0, 0, 10},
    {0, 0, 0} };*/

    int programs[LINE][COLUN] =  { 
        {  202,    0, -471, -149,  -73,  350,  499,  616,  -85,  611,  444, -484, -228, -265,  572, -138, -169,  442, -380,  213 },
        {  201,    0, -280,  428, -323,  -85,  658,  355, -457, -158,   39,   18,  777,  732,  719, -445,  209, -451,    0,  -93 },
        {  102,    0,    6,   84,  341,    0,  443, -221,  623, -157,  499, -256,   51,  -94, -187, -263, -362, -309,    0, -159 },
        {    4,    0,  666,  610, -192,    0,    0,  793,  668, -451,    0,  368,    8,  752,  455, -487, -258,  780,    0,  361 },
        {    0,    0,   10,  748,  441,    0,    0,  -85,  529,  634,    0, -291,  672, -336,  728,    0,  792,  447,    0,  214 },
        {    0,    0,    0,  -84,  -59,    0,    0, -100, -163,  519,    0,  625, -152, -305,  721,    0,    0,  382,    0, -136 },
        {    0,    0,    0, -413,  479,    0,    0,  517,  -63,  -48,    0, -385, -359,    0,  517,    0,    0,  636,    0, -488 },
        {    0,    0,    0,  539,  737,    0,    0,  -79,    0,  570,    0,  553,  212,    0,    0,    0,    0,    0,    0,  772 },
        {    0,    0,    0, -253,  -19,    0,    0, -318,    0,  347,    0,    0,   87,    0,    0,    0,    0,    0,    0, -257 },
        {    0,    0,    0,  628,  326,    0,    0,    0,    0,  420,    0,    0,  393,    0,    0,    0,    0,    0,    0,   -4 },
        {    0,    0,    0, -101,  -46,    0,    0,    0,    0, -319,    0,    0,  534,    0,    0,    0,    0,    0,    0, -175 }};
    Process *process;
    Process *processor = NULL;
  
    Queue *NewQueue = createQueue();
    Queue *BloquedQueue  = createQueue();
    Queue *ReadyQueue = createQueue();
    Queue *ExitQueue = createQueue();

    process = forks(programs[0], &globalPid, time);
    enqueue(NewQueue, process);

    // Cabeçalho da tabela
    fprintf(file, "%-10s", "time inst");
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        char label[16];
        sprintf(label, "proc%d", i);
        fprintf(file, "%-15s", label);
    }
    fprintf(file, "\n");

    while (time < TIME_LIMITE) 
    {
        time++;
        printProcess(file, processor, NewQueue, BloquedQueue, ReadyQueue, ExitQueue);

        scheduler(BloquedQueue, ReadyQueue, READY);
        scheduler(NewQueue, ReadyQueue, READY);
        scheduler(ExitQueue, ReadyQueue, EXIT);

        if (processor == NULL) 
        {
            Process *auxProcess = (Process *)dequeue(ReadyQueue);
            if (auxProcess != NULL) 
            {
                auxProcess->status = RUNNING;
                processor = auxProcess;
                processor->timeCpu = 2;
            }
        } else {
            int index = processor->programCounter;
            int pc = processor->context[index];

            if (processor->timeCpu <= QUANTUM) 
            {
                if (processor->programCounter > COLUN) 
                {
                    dispatcher(processor, ExitQueue, EXIT);
                    processor = NULL;
                    continue;
                } else {
                    if (pc > 100 && pc < 200) 
                    {
                        int jump = pc % 100;
                        processor->programCounter -= jump;
                    } else if (pc > 200 && pc < 300) {
                        int idx = pc % 100;
                        if (globalPid <= MAX_PROCESS) 
                        {
                            Process *forkProcess = forks(programs[idx], &globalPid, time);
                            if (forkProcess != NULL) enqueue(NewQueue, forkProcess);
                        }
                    } else if (pc < 0) {
                        dispatcher(processor, BloquedQueue, BLOCKED);
                        processor = NULL;
                    } else if (pc == 0) {
                        dispatcher(processor, ExitQueue, EXIT);
                        processor = NULL;
                    }
                }
            } else {
                if (processor->programCounter > COLUN) 
                {
                    dispatcher(processor, ExitQueue, EXIT);
                    processor = NULL;
                    continue;
                } else {
                    dispatcher(processor, ReadyQueue, READY);
                    processor = NULL;
                }
            }

            if (processor != NULL) 
            {
                processor->programCounter += 1;
                processor->timeCpu += 1;
            }
        }
        if (isEmpty(NewQueue) && isEmpty(BloquedQueue) && isEmpty(ReadyQueue) && processor == NULL)
            break;
    }
    deleteQueue(ReadyQueue);
    deleteQueue(ExitQueue);
    deleteQueue(BloquedQueue);
    deleteQueue(NewQueue);

    if(processor != NULL)
    {
        processor->status = EXIT;
        killProcess(processor);
    }

    fclose(file);
    return 0;
}