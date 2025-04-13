#include "process.h"
#include <stdio.h>
#include "queue.h"
#include <string.h>

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
Process *forks(int context[], int *pid)
{
    Process *process = (Process *)malloc(sizeof(Process));

    if ( process == NULL )
    {
        error_code(NO_SPACE);
        return NULL;
    }

    memcpy(process->context, context, COLUN * sizeof(int)); 
    process->index = 0;
    process->pid = pid; 
    process->timeEnqueue = 1;
    process->timeExec = 0;
    process->status = NEW;

    pid++;
    return process;
}

/*
    kill process, this is a call 
*/
void kill(Process *process)
{
    if( process->status != EXIT )
    {
        error_code(INVALID_CALL);
        return;
    }

    free(process);
} 

/*
    print all process
*/
//void print_process(Queue *New, Queue *Bloqued, Queue *Ready, Process *processor)


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