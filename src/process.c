#include "process.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    message and error status 
*/
void error_code(Signal flag)
{
    switch ( flag )
    {
        case INVALID_NUMBER:
            printf(".....");
            break;
        case SUCESS:
            printf(" ");
            break;
        case NO_SPACE:
            printf(" ");
            break;
        case INVALID_CALL:
            printf(" ");
            break;
        default:
            printf("Warning: !!!");
            break;
    }
}

/*
    create process, this is a call 
*/
Process *exec(int context[], int pid, int time)
{
    Process *process = (Process *)malloc(sizeof(Process));

    if ( process == NULL )
    {
        error_code(NO_SPACE);
        return NULL;
    }

    memcpy(process->context, context, COLUN * sizeof(int)); 
    process->programCounter = 0; // pc of the process
    process->pid = pid; // pid of the process
    process->timeEnqueue = time + 2; // time with process stay in any queue, this is generics
    process->timeCpu = 1; // this time increment while process stay in the cpu
    process->status = NEW; 

    return process;
}

/*
    kill process, this is a call 
*/
void killProcess(Process *process)
{
    if( process->status != EXIT || process == NULL )
    {
        error_code(INVALID_CALL);
        return;
    }

    free(process);
} 

/*
    if necessary other service call the system
*/
void system_call(char call)
{
    switch (call)
    {
        case 'f':
            break;
        
        default:
            break;
    }
}

/*
*/
const char *getStatus(Status status) 
{    
    switch (status) 
    {
        case NEW:
            return "NEW";
        case BLOCKED:
            return "BLOCKED";
        case READY:
            return "READY";
        case RUNNING:
            return "RUN";
        case EXIT:
            return "EXIT";
        default:
            return "UNKNOWN";
    }
}