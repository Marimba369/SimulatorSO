// Trabalho 1 de Sistemas Operativos
// Simulador com modelo de 5 estados e escalonamento Round Robin (Quantum = 3)

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

#define QUANTUM 3
#define TIME_LIMITE 100
#define NUMBER_PROCESS 5
#define MAX_PROCESS 20

int time = 0;
int globalPid = 1;

void scheduler(Queue *QueueDestiny, Queue *Ready, Status status);
Process *findByTime(Queue *Queue);
Process *findByPID(Queue *Queue, int pid);

Process *findByTime(Queue *Queue) {
    if (isEmpty(Queue)) return NULL;
    for (QueueNode *node = Queue->front; node != NULL; node = node->next) {
        Process *process = (Process *)node->data;
        if (process->timeEnqueue == time)
            return process;
    }
    return NULL;
}

Process *findByPID(Queue *Queue, int pid) {
    if (isEmpty(Queue)) return NULL;
    for (QueueNode *node = Queue->front; node != NULL; node = node->next) {
        Process *process = (Process *)node->data;
        if (process->pid == pid)
            return process;
    }
    return NULL;
}

void scheduler(Queue *QueueDestiny, Queue *Ready, Status status) {
    Process *process = findByTime(QueueDestiny);
    if (process == NULL) return;

    if (process->status == NEW || process->status == BLOCKED) {
        process = (process->status == NEW) ? (Process *)dequeue(QueueDestiny)
                                           : removeNodeByData(QueueDestiny, process);
        process->status = status;
        enqueue(Ready, process);
    } else if (process->status == EXIT) {
        Process *toFree = dequeue(QueueDestiny);
        killProcess(toFree);
    }
    scheduler(QueueDestiny, Ready, status);
}

void dispatcher(Process *processor, Queue *QueueDestiny, Status status) {
    processor->status = status;
    processor->timeCpu = 0;

    if (status == BLOCKED) {
        int pc = processor->programCounter;
        int timeEnqueue = (processor->context[pc] * -1) + time;
        processor->timeEnqueue = timeEnqueue;
    } else if (status == EXIT) {
        processor->timeEnqueue = time + 1;
    }
    enqueue(QueueDestiny, processor);
}

void printProcess(Process *processor, Queue *New, Queue *Blocked, Queue *Ready, Queue *Exit) {
    printf("%-10d", time);
    for (int i = 1; i <= NUMBER_PROCESS; i++) {
        if (processor != NULL && processor->pid == i) {
            printf("%-15s", getStatus(processor->status));
        } else {
            Process *process = findByPID(Blocked, i);
            if (process != NULL) printf("%-15s", getStatus(process->status));
            else if ((process = findByPID(Ready, i)) != NULL) printf("%-15s", getStatus(process->status));
            else if ((process = findByPID(New, i)) != NULL) printf("%-15s", getStatus(process->status));
            else if ((process = findByPID(Exit, i)) != NULL) printf("%-15s", getStatus(process->status));
            else printf("%-15s", "");
        }
    }
    printf("\n");
}

int main() {
    int programs[5][20] = {
        { 203, 4, 2 },
        { 1, -3, 202 },
        { -2, 1, 0 },  // corrigido para evitar loop infinito
        { 1, 0, 10 },
        { 0, 0, 0 }
    };

    Process *process;
    Process *processor = NULL;

    Queue *NewQueue = createQueue();
    Queue *BloquedQueue = createQueue();
    Queue *ReadyQueue = createQueue();
    Queue *ExitQueue = createQueue();

    process = forks(programs[0], &globalPid, time);
    enqueue(NewQueue, process);

    printf("%-10s", "time inst");
    for (int i = 1; i <= NUMBER_PROCESS; i++) {
        char label[16];
        sprintf(label, "proc%d", i);
        printf("%-15s", label);
    }
    printf("\n");

    while (time < TIME_LIMITE) {
        time++;
        printProcess(processor, NewQueue, BloquedQueue, ReadyQueue, ExitQueue);

        scheduler(BloquedQueue, ReadyQueue, READY);
        scheduler(NewQueue, ReadyQueue, READY);
        scheduler(ExitQueue, ReadyQueue, EXIT);

        if (processor == NULL) {
            Process *auxProcess = (Process *)dequeue(ReadyQueue);
            if (auxProcess != NULL) {
                auxProcess->status = RUNNING;
                processor = auxProcess;
                processor->timeCpu = 1;
            }
        } else {
            int index = processor->programCounter;
            int pc = processor->context[index];

            if (processor->timeCpu <= QUANTUM) {
                if (processor->programCounter >= 20) {
                    dispatcher(processor, ExitQueue, EXIT);
                    processor = NULL;
                    continue;
                } else {
                    if (pc > 100 && pc < 200) {
                        int jump = pc % 100;
                        processor->programCounter -= jump;
                    } else if (pc > 200 && pc < 300) {
                        int idx = pc % 100;
                        if (globalPid <= MAX_PROCESS) {
                            Process *forkProcess = forks(programs[idx], &globalPid, time);
                            if (forkProcess != NULL) enqueue(NewQueue, forkProcess);
                        }
                    } else if (pc < 0) {
                        dispatcher(processor, BloquedQueue, BLOCKED);
                        processor = NULL;
                    } else if (pc == 0) {
                        dispatcher(processor, ExitQueue, EXIT);
                        processor = NULL;
                    }
                }
            } else {
                dispatcher(processor, ReadyQueue, READY);
                processor = NULL;
            }

            if (processor != NULL) {
                processor->programCounter += 1;
                processor->timeCpu += 1;
            }
        }

        // Verifica se não há mais processos ativos
        if (isEmpty(NewQueue) && isEmpty(BloquedQueue) && isEmpty(ReadyQueue) && processor == NULL)
            break;
    }

    return 0;
}
