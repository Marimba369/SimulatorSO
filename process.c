#include "process.h"
#include <stdio.h>
#include "queue.h"


/*

*/
Signal insertDataProcess(Process process[LEN], int programs[][COLUN])
{
    if (LEN < COLUN || LEN > LINE) 
        return INVALID_NUMEBER;

    for (int i = 0; i < LEN; i++)
    {
        memcpy(process[i].data, programs[i], COLUN * sizeof(int)); 
        process[i].point = 0;
        process[i].pid = i + 1; 
        process[i].status = READY;
    }

    return SUCESS;
}


/*

*/
void error_code(Signal flag)
{
    switch ( flag )
    {
        case INVALID_NUMEBER:
            printf(".....");
            break;
        case SUCESS:
            printf(" ");
            break;
        default:
            break;
    }
}

/*

*/
void printProcess(Process process[])
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
void insertProcess(Queue *queue, Process *process, int len)
{
    for(int i = 0; i < len; i++)
        enqueue(queue, process + i);
}