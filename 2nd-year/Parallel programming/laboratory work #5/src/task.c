//
// Created by xp10rd on 5/13/19.
//

#include "task.h"

p_problem *p_init(int task_cycles, int task_num) {
    p_problem *p;

    if((p = malloc(sizeof(p_problem))) == NULL) {
        return NULL;
    }

    MPI_Type_contiguous(1, MPI_INT, &p->task_type);
    MPI_Type_commit(&p->task_type);

    if((p->rez = malloc(sizeof(p_result))) == NULL) {
        free(p);
        return NULL;
    }

    if((p->q = q_init_queue()) == NULL) {
        free(p->rez);
        free(p);
        return NULL;
    }

    if(pthread_mutex_init(&p->p_mutex, NULL) != 0) {
        free(p->rez);
        q_free(p->q, p_free_task);
        free(p);
        return NULL;
    }
    if(pthread_cond_init(&p->p_wait_cond, NULL) != 0) {
        free(p->rez);
        q_free(p->q, p_free_task);
        free(p);
        return NULL;
    }
    p->task_cycles = task_cycles;
    p->in_q_task_max_num = task_num;

    return p;
}

void p_free(p_problem *p) {
    if(p->rez) {
        free(p->rez);
    }
    if(p->q) {
        q_free(p->q, p_free_task);

    }
    free(p);
}

void p_run_task(p_task *t, p_problem *p) {
    for(int i = 0; i < t->weight; i++) {
        p->rez->sum += sqrt(i);
    }
}

int p_is_any_task_cycles(p_problem *p, pthread_mutex_t **p_mutex) {
    pthread_mutex_lock(&p->p_mutex);
    *p_mutex = &p->p_mutex;
    return p->task_cycles;
}

int p_dec_task_cycles(p_problem *p, pthread_mutex_t **p_mutex) {
    pthread_mutex_lock(&p->p_mutex);
    *p_mutex = &p->p_mutex;
    return --p->task_cycles;
}

p_task *p_gen_task(int rank, int size, int task_num, void *data) {
    p_task *t;

    if((t = malloc(sizeof(p_task))) == NULL) {
        return NULL;
    }
    t->weight = rank * task_num * abs(rank - (*((int*)data) % size));

    return t;
}

p_task *p_get_no_tasks_struct() {
    p_task *no_task_stucture;

    if((no_task_stucture = malloc(sizeof(p_task))) == NULL) {
        return NULL;
    }
    no_task_stucture->weight = -1;

    return no_task_stucture;
}

int p_comp_tasks(p_task *t1, p_task *t2) {
    return (t1->weight == t2->weight);
}

void p_print_result(p_result *rez) {
    printf("p_result: %lf\n", rez->sum);
}

void p_free_task(void *t) {
    free(t);
}

p_result *p_reduce_result(MPI_Comm comm, p_problem *p) {
    p_result *global_rez;

    if((global_rez = malloc(sizeof(p_result))) == NULL) {
        return NULL;
    }

    global_rez->sum = 0;
    MPI_Allreduce(&p->rez->sum, &global_rez->sum, 1, MPI_DOUBLE, MPI_SUM, comm);

    return global_rez;
}


void p_generate_tasks(p_problem *p, int size, int rank, void *coeff) {
    for(int i = 0; i < p->in_q_task_max_num; i++) {
        q_push(p->q, p_gen_task(rank, size, i, coeff));
    }
}

void p_copy_task(p_task *dest, p_task *src) {
    dest->weight = src->weight;
}

pthread_mutex_t *p_wait(p_problem *p) {
    pthread_mutex_lock(&p->p_mutex);
    pthread_cond_wait(&p->p_wait_cond, &p->p_mutex);
    return &p->p_mutex;
}

void p_signal(p_problem *p) {
    pthread_cond_signal(&p->p_wait_cond);
}