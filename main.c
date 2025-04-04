#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"


int main()
{
    int programs[LINE][COLUN] = { {3, 4, 2},
    {1, 2, 1},
    {2, 4, 5},
    {0, 0, 10},
    {0, 0, 0} };

    Process process;
    init(&process, programs[0]);

    for(int i = 0; i < COLUN; i++)
        printf(" %d ", process.data[i]);

    return 0;
}