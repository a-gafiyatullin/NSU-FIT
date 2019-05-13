//
// Created by xp10rd on 5/13/19.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
#include "task.h"
#include <mpi.h>

#define REQ_TASK 0
#define REQ_END 1
#define REQ_IN 1
#define REQ_OUT 0

#define MINIMAL_IN_QUEUE_TASKS_NUM 2

typedef struct scheduler {
    queue *q;
    int comm_size;
    int rank;
} scheduler;

void *in_scheduler(void *arg);

void *out_scheduler(void *arg);

#endif //SCHEDULER_H
