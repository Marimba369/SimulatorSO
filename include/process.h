#ifndef PROCESS_H
#define PROCESS_H

#define LINE 11
#define COLUN 20

typedef enum { INVALID_NUMBER,  SUCESS, NO_SPACE, INVALID_CALL } Signal;
typedef enum { NEW, READY, RUNNING, BLOCKED, EXIT } Status;

typedef struct{
    int pid;
    int context[COLUN];
    int programCounter;
    int timeEnqueue; 
    int timeCpu;
    Status status;
} Process;    


Process *exec(int programs[], int pid, int time);
void error_code(Signal flag);
void killProcess(Process *process);
const char *getStatus(Status status);

#endif