#ifndef TASK_H
#define TASK_H

#include <math.h>
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

typedef struct p_task {
    int weight;
} p_task;

typedef struct p_result {
    double sum;
} p_result;

typedef struct problem {
    pthread_mutex_t p_mutex;
    pthread_cond_t p_wait_cond;
    queue *q;
    p_result *rez;
    int task_cycles;
    int in_q_task_max_num;
    MPI_Datatype task_type;
} p_problem;

p_problem *p_init(int task_cycles, int task_num);

void p_free(p_problem *p);

void p_run_task(p_task *t, p_problem *p);

p_task *p_gen_task(int rank, int size, int task_num, void *data);

int p_is_any_task_cycles(p_problem *p, pthread_mutex_t **p_mutex);

int p_dec_task_cycles(p_problem *p, pthread_mutex_t **p_mutex);

p_task *p_get_no_tasks_struct();

int p_comp_tasks(p_task *t1, p_task *t2);

void p_print_result(p_result *rez);

void p_free_task(void *t);

p_result *p_reduce_result(MPI_Comm comm, p_problem *p);

void p_generate_tasks(p_problem *p, int size, int rank, void *coeff);

void p_copy_task(p_task *dest, p_task *src);

pthread_mutex_t *p_wait(p_problem *p);

void p_signal(p_problem *p);

#endif //TASK_H