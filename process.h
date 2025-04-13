#ifndef PROCESS_H
#define PROCESS_H

#define LINE 5
#define COLUN 3
#define LEN 5

#include "queue.h"

typedef enum { INVALID_NUMBER,  SUCESS, NO_SPACE, INVALID_CALL } Signal;
typedef enum { NEW, READY, RUNNING, BLOCKED, EXIT } Status;

typedef struct{
    int pid;
    int context[COLUN];
    int index;
    int timeEnqueue;
    int timeExec;
    Status status;
} Process;    


Process *forks(int programs[], int *pid);
void error_code(Signal flag);
void kill(Process *process);
void print_process(Queue *New, Queue *Bloqued, Queue *Ready, Process *processor);


#endif