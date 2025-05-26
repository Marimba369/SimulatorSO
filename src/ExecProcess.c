#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"

#define QUANTUM 3
#define TIME_LIMITE 100
#define NUMBER_PROCESS 20
#define MAX_PROCESS 20

int time = 1;

void scheduler(Queue *QueueSource, Queue *Ready, Status status);
void dispatcher(Process **processor, Queue *QueueDestiny, Status status, Queue *QueueReady);
Process *findByTime(Queue *Queue);
Process *findByPID(Queue *Queue, int pid);
void printProcess(FILE *file, Process *processor, Queue *New, Queue *Blocked, Queue *Ready, Queue *Exit);


/*
 * Search for element by time 
 * return: element address if exists else null 
*/
Process *findByTime(Queue *Queue)
{
    if( isEmpty(Queue) )
        return NULL;
    for( QueueNode *node = Queue->front; node != NULL; node = node->next )
    {
        Process *process = (Process *)node->data;
        if( process->timeEnqueue == time )
            return process; // find element
    }
    return NULL; // no element with time-out
}

/*
 * Search for element by PID
 * return: element address if exists else null 
*/
Process *findByPID(Queue *Queue, int pid)
{     
    if( isEmpty(Queue) )
        return NULL;
    for( QueueNode *node = Queue->front; node != NULL; node = node->next )
    {
        Process *process = (Process *)node->data;
        if( process->pid == pid )
            return process; // find element
    }
    return NULL; // no element with time-out
}

/*
 * scheduler: Moves processes from source queues (New, Blocked) to Ready queue,
 * or handles processes in Exit queue.
*/
void scheduler(Queue *QueueSource, Queue *Ready, Status status)
{
    while (1) 
    {
        Process *process = findByTime(QueueSource); 
        if (process == NULL)
            break;

        // Use removeNodeByData to ensure the *found* process is removed from the source queue
        Process *removedProcess = (Process *)removeNodeByData(QueueSource, process);

        if (removedProcess == NULL) {
            // This should ideally not happen if findByTime returned a process,
            // but it's a safeguard against unexpected queue state.
            break; 
        }

        // Handle processes based on their status after removal
        if (removedProcess->status == EXIT) 
        {
            killProcess(removedProcess); // Free resources for exited processes
            continue; // Continue checking for more processes in the source queue
        } 
        else { // This covers NEW and BLOCKED states that are moved to READY
            // Update status and enqueue to Ready queue
            removedProcess->status = status;
            removedProcess->timeCpu = 1; // Reset CPU time for a new quantum
            enqueue(Ready, removedProcess);
        }
    }
}


/*
 * dispatcher: Handles moving the current process off the CPU and bringing a new one on.
*/
void dispatcher(Process **processor, Queue *QueueDestiny, Status status, Queue *QueueReady)
{
    if( *processor != NULL )
    {
        (*processor)->status = status;
        // Reset timeCpu for the process leaving the CPU, if it's going back to READY or BLOCKED.
        if (status == READY || status == BLOCKED) {
            (*processor)->timeCpu = 1; // Reset for next quantum
        }

        if( status == BLOCKED )
        {
            int address = (*processor)->programCounter;
            // Calculate timeEnqueue for blocked processes
            // If context[address] is -X, then timeEnqueue = X + current_time
            int timeEnqueue = ((*processor)->context[address] * -1) + time;
            (*processor)->timeEnqueue = timeEnqueue;
        }
        else if( status == EXIT )
        {
            // Set timeEnqueue for processes going to ExitQueue (for collection at next time step)
            (*processor)->timeEnqueue = time + 1;
        }
        
        // Enqueue the current process to its destination queue
        enqueue(QueueDestiny, *processor); // CORRECTED: Pass the Process*
    }
 
    // Get a new process from the ReadyQueue to run on the CPU
    *processor = (Process *)dequeue(QueueReady);

    if( *processor == NULL )
        return; // No process available to run

    // Set the status of the newly dispatched process to RUNNING
    (*processor)->status = RUNNING;
    // programCounter is NOT incremented here; it points to the next instruction to execute.
    // Incrementing happens in main after instruction execution.
} 

/*
 * printProcess: Prints the status of all processes at the current time step.
*/
void printProcess(FILE *file, Process *processor, Queue *New, Queue *Blocked, Queue *Ready, Queue *Exit)
{
    fprintf(file, "%-10d", time); 
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        if (processor != NULL && processor->pid == i) {
            fprintf(file, "%-15s", getStatus(processor->status));
        } else {
            Process *process = findByPID(Blocked, i);
            if (process != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else if ((process = findByPID(Ready, i)) != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else if ((process = findByPID(New, i)) != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else if ((process = findByPID(Exit, i)) != NULL) {
                fprintf(file, "%-15s", getStatus(process->status));
            } else {
                fprintf(file, "%-15s", ""); // Process not found in any active queue
            }
        }
    }
    fprintf(file, "\n");
}


/*
 * main: Main simulation loop for the process scheduler.
*/
int main()
{
    int globalPid = 1;

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Error opening output file");
        return 1;
    }

    // Define the programs (instruction sets)
    // LINE = 5, COLUN = 3 from process.h
    /*int programs[LINE][COLUN] = { 
        {204, 202, 3},  // Program 0: Fork (id=4), Fork (id=2), instruction 3
        {1, 204, 1},    // Program 1: instruction 1, Fork (id=4), instruction 1
        {2, 201, 5},    // Program 2: instruction 2, Fork (id=1), instruction 5
        {0, 0, 10},     // Program 3: instruction 0, instruction 0, instruction 10 (exit)
        {0, 0, 0}       // Program 4: instruction 0, instruction 0, instruction 0 (exit)
    };*/

    int programs[LINE][COLUN] = { 
    {  215,  203,  201,    2,  205,  207,    3,  210,    5,  300,  214,  400,  -20,  500,  220,  600,  -30,  700,    9,  216 },
    {  202,  204,    2,    4,  206,  208,    8,  209,   10,  310,  212,  320,   14,  330,  219,  340,   18,  350,   20,  217 },
    {    1,    3,    3,    5,    7,    9,   11,  211,   15,   17,  213,   21,   23,   25,  218,   29,   31,   33,   35,   37 },
    {   -3,   23, 3443, 4554, 5665,  565,   67,   67,   87,   98,    6,    7,    9,   10,   70,   60,   10,   20,   20,   30 },
    {  -50,   55,  -60,  -65,  -70,   75,  -80,  -85,  -90,  -95, -100,  200, -250, -300, -350, -400, -450, -500, -550,   -6 },
    {  200,  205,  210,  215,  310,  315,  320,  325,  330,  335,  340,  345,  350,  355,  360,  365,  370,  375,  380,  385 },
    {   50,    0,    0,    0,    0,    0,    0,    0,    0,    0,   54,    0,    0,    0,    0,    0,    0,    0,    0,  443 },
    {  -25,    0,    0,    0,    0,    0,    0,    0,    0,    0,   44,    0,    0,    0,    0,    0,    0,    0,    0,    5 },
    {  250,    0,    0,    0,    0,    0,    0,    0,    0,    0,    3,    0,    0,    0,    0,    0,    0,    0,    0, 3443 },
    {  400,    0,    0,    0,    0,    0,    0,    0,    0,    0,    6,    0,    0,    0,    0,    0,    0,    0,    0,    3 },
    {    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 }};
    
    Process *process;
    Process *processor = NULL; // Current process on the CPU
 
    // Initialize queues
    Queue *NewQueue = createQueue();
    Queue *BloquedQueue  = createQueue();
    Queue *ReadyQueue = createQueue();
    Queue *ExitQueue = createQueue();

    // Create the initial process (Program 0) and add to NewQueue
    process = exec(programs[0], globalPid, time);
    enqueue(NewQueue, process);
    globalPid++; // Increment global PID for the next process

    // Print table header
    fprintf(file, "%-10s", "time inst");
    for (int i = 1; i <= NUMBER_PROCESS; i++) 
    {
        char label[16];
        sprintf(label, "proc%d", i);
        fprintf(file, "%-15s", label);
    }
    fprintf(file, "\n");

    // Main simulation loop
    while( time <= TIME_LIMITE )
    {
        // Schedule processes from Blocked, New, and Exit queues
        scheduler(BloquedQueue, ReadyQueue, READY); // Blocked -> Ready
        scheduler(NewQueue, ReadyQueue, READY);     // New -> Ready
        scheduler(ExitQueue, NULL, EXIT);           // Exit queue processes are killed (NULL for Ready is fine here)    

        // Check if there are any active processes left in the system
        if (isEmpty(NewQueue) && isEmpty(BloquedQueue) && isEmpty(ReadyQueue) && processor == NULL && isEmpty(ExitQueue))
            break; // All processes are finished

        // If no process is currently running on the CPU, try to dispatch one from ReadyQueue
        if (processor == NULL)
        {
            processor = (Process *)dequeue(ReadyQueue);
            if (processor != NULL) {
                processor->status = RUNNING;
                processor->timeCpu = 0;
                // timeCpu is already 0 when it enters ReadyQueue via scheduler/dispatcher
            }
        }
        
        // If there's a process on the CPU, execute its instruction or dispatch it
        if (processor != NULL)
        {
            // Check if quantum expired or process finished its program
            if (processor->timeCpu >= QUANTUM || processor->programCounter >= COLUN)
            {
                Queue *QueueDestiny;
                Status status;

                if (processor->programCounter >= COLUN) { // Process finished its program
                    QueueDestiny = ExitQueue;
                    status = EXIT;
                } else { // Quantum expired
                    QueueDestiny = ReadyQueue;
                    status = READY;
                }
                
                // Dispatch the current process and get a new one (if available)
                dispatcher(&processor, QueueDestiny, status, ReadyQueue);
            }
            else // Execute the current instruction
            {
                int address = processor->programCounter;
                int instruction = processor->context[address];

                if (instruction > 100 && instruction < 200) // Jump instruction (e.g., 1XX)
                {
                    int jump = instruction % 100;
                    // Ensure jump doesn't go to a negative address. If it would, jump to 0.
                    processor->programCounter = (address - jump < 0) ? 0 : (address - jump);
                    
                    processor->programCounter += 1; // Move to next instruction after jump
                    processor->timeCpu += 1;
                }
                else if (instruction > 200 && instruction < 300) // Fork instruction (e.g., 2XX)
                {
                    int id = instruction % 100; // Get the program ID to fork
                    if (id < LINE && globalPid <= MAX_PROCESS) // Check if program ID is valid and max processes not reached
                    {
                        Process *forkProcess = exec(programs[id], globalPid, time);
                        if (forkProcess != NULL) 
                        {
                            enqueue(NewQueue, forkProcess);
                            globalPid++;
                        }
                    }
                    
                    processor->programCounter += 1;
                    processor->timeCpu += 1;
                }
                else if (instruction <= 0) // Block (negative) or Exit (0) instruction
                {
                    Queue *QueueDestiny = (instruction < 0) ? BloquedQueue : ExitQueue;
                    Status status = (instruction < 0) ? BLOCKED : EXIT; 
                    
                    dispatcher(&processor, QueueDestiny, status, ReadyQueue);
                }
                else // Regular instruction (positive, non-special)
                {
                    processor->programCounter += 1;
                    processor->timeCpu += 1;
                }
            }
        }
        // Print the status of all processes for the current time step
        printProcess(file, processor, NewQueue, BloquedQueue, ReadyQueue, ExitQueue);
        fflush(file);
        time++; // Advance simulation time
    }

    // Clean up remaining queues and the processor
    deleteQueue(ReadyQueue);
    deleteQueue(ExitQueue);
    deleteQueue(BloquedQueue);
    deleteQueue(NewQueue);

    if(processor != NULL)
    {
        processor->status = EXIT; // Mark as exit before killing
        killProcess(processor);
    }

    fclose(file); // Close the output file
    return 0;
}
