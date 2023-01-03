//
// Created by xp10rd on 5/13/19.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
#include "task.h"
#include <mpi.h>

#define REQ_OUT 0
#define REQ_IN 1

#define REQ_END 0
#define REQ_TASK 1

typedef struct scheduler {
    p_problem *p;
    int comm_size;
    int rank;
    int send_task_num;
} scheduler;

void *in_scheduler(void *arg);

void *out_scheduler(void *arg);

void *simple_out_scheduler(void *arg);

#endif //SCHEDULER_H
