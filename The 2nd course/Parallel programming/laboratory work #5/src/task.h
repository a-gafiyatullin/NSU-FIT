//
// Created by xp10rd on 5/13/19.
//

#ifndef TASK_H
#define TASK_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define RAND 10
#define TASKS_NUM 1
#define TASKS_CYCLES 1

MPI_Datatype task_type;
extern int task_cycles;

typedef struct task {
    int weight;
} task;

typedef struct result {
    double sum;
} result;

void tasks_init();

void result_init(result *rez);

void run_task(task *t, result *rez);

task *gen_task(int rank, int size, void *data);

int is_any_task_cycles();

int decrease_task_cycles();

task get_no_tasks_structure();

int compare_tasks(task *t1, task *t2);

void print_result(result *rez);

#endif //TASK_H