#include "process.h"
#include <stdio.h>
#include "queue.h"
#include<string.h>

/*

*/
Signal init(Process *process, int programs[])
{
    if (LEN < COLUN || LEN > LINE) 
        return INVALID_NUMBER;

    memcpy(process->data, programs, COLUN * sizeof(int)); 
    process->point = 0;
    process->pid = 1; 
    process->timeEnqueue = 0;
    process->status = NEW;
        
    return SUCESS;
}


/*

*/
void printProcess(Process *process)
{
    for(int i = 0; i < LEN; i++)
    {
        printf("%d \t", i + 1);
        for(int j = 0; j < COLUN; j++)
            printf(" %d ", process[i].data[j]);
        printf("\n");
    }
}



/*

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
        default:
            break;
    }
}