#ifndef PROCESS_H
#define PROCESS_H

#define LINE 5
#define COLUN 3
#define LEN 5

#include "queue.h"

typedef enum { INVALID_NUMEBER,  SUCESS } Signal;
typedef enum { READY, RUN } Status;

typedef struct{
    int data[COLUN];
    int point;
    int pid;
    Status status;
} Process;


Signal insertDataProcess(Process process[LEN], int programs[][COLUN]);
void error_code(Signal flag);
void printProcess(Process process[]);
void insertProcess(Queue *queue, Process *process, int len);


#endif