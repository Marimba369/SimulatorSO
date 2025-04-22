#ifndef PROCESS_H
#define PROCESS_H

#define LINE 5
#define COLUN 20
#define LEN 11

#include "queue.h"

typedef enum { INVALID_NUMBER,  SUCESS, NO_SPACE, INVALID_CALL } Signal;
typedef enum { NEW, READY, RUNNING, BLOCKED, EXIT, DUMP } Status;

typedef struct{
    int pid;
    int context[COLUN];
    int programCounter;
    int timeEnqueue; 
    int timeCpu;
    //int flag;
    Status status;
} Process;    


Process *forks(int programs[], int *pid, int time);
void error_code(Signal flag);
void killProcess(Process *process);
char *getStatus(Status status);
//void print_process(Queue *New, Queue *Bloqued, Queue *Ready, Process *processor);


#endif