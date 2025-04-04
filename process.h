#ifndef PROCESS_H
#define PROCESS_H

#define LINE 5
#define COLUN 3
#define LEN 5

#include "queue.h"

typedef enum { INVALID_NUMBER,  SUCESS } Signal;
typedef enum { NEW, READY, RUNNING, BLOCKED, EXIT } Status;

typedef struct{
    int pid;
    int data[COLUN];
    int point;
    int timeEnqueue;
    Status status;
} Process;


Signal init(Process *process, int programs[]);
void error_code(Signal flag);
void printProcess(Process *process);


#endif