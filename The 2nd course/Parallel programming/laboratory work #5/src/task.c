//
// Created by xp10rd on 5/13/19.
//

#include "task.h"

int task_cycles  = TASKS_CYCLES;

void tasks_init() {
    MPI_Type_contiguous(1, MPI_INT, &task_type);
    MPI_Type_commit(&task_type);
}

void run_task(task *t, result *rez) {
    for(int i = 0; i < t->weight; i++) {
        rez->sum += sqrt(i);
    }
}

void result_init(result *rez) { rez->sum = 0; }

int is_any_task_cycles() { return task_cycles; }

int decrease_task_cycles() { return --task_cycles; }

task *gen_task(int rank, int size, void *data) {
    task *t = malloc(sizeof(task));
    t->weight = 2;/*(rand() % RAND) * abs(rank - (*((int*)data) % size))*/;

    return t;
}

task get_no_tasks_structure() {
    task no_task_stucture;
    no_task_stucture.weight = -1;

    return no_task_stucture;
}

int compare_tasks(task *t1, task *t2) {
    if(t1->weight != t2->weight) {
        return 0;
    }

    return 1;
}

void print_result(result *rez) {
    printf("result: %lf\n", rez->sum);
}