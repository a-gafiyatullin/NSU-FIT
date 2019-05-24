//
// Created by xp10rd on 5/13/19.
//

#include "scheduler.h"

void *in_scheduler(void *arg) {

    scheduler *schdlr = (scheduler*)arg;
    int req_type = 0;
    MPI_Status status;
    p_task *t, *no_task = p_get_no_tasks_struct();
    pthread_mutex_t *q_mutex;
    p_task send_tasks[schdlr->send_task_num];

    while (1) {
        MPI_Recv(&req_type, 1, MPI_INT, MPI_ANY_SOURCE, REQ_IN, MPI_COMM_WORLD, &status);
        if(req_type == REQ_TASK) {
            if(q_get_counter(schdlr->p->q, &q_mutex) <= schdlr->send_task_num) {
                pthread_mutex_unlock(q_mutex);
                p_copy_task(send_tasks, no_task);
            } else {
                pthread_mutex_unlock(q_mutex);
                for(int i = 0; i < schdlr->send_task_num; i++) {
                    t = q_pop(schdlr->p->q);
                    if(t != NULL) {
                        p_copy_task(send_tasks + i, t);
                        p_free_task(t);
                    } else {
                        p_copy_task(send_tasks + i, no_task);
                        break;
                    }
                }
            }
            MPI_Send(send_tasks, schdlr->send_task_num, schdlr->p->task_type, status.MPI_SOURCE, REQ_OUT, MPI_COMM_WORLD);
        } else if(req_type == REQ_END) {
            break;
        }
    }

    p_free_task(no_task);
}

void *out_scheduler(void *arg) {

    scheduler *schdlr = (scheduler*)arg;
    int i, tmp, coeff, j;
    int req_type = REQ_TASK;
    p_task *no_task = p_get_no_tasks_struct();
    pthread_mutex_t *q_mutex, *p_mutex;
    p_task recv_tasks[schdlr->send_task_num];

    while (1) {
        p_mutex = p_wait(schdlr->p);
        pthread_mutex_unlock(p_mutex);
        for(i = 0; i < schdlr->comm_size; i++) {
            if(i != schdlr->rank) {
                MPI_Send(&req_type, 1, MPI_INT, i, REQ_IN, MPI_COMM_WORLD);
                MPI_Recv(recv_tasks, schdlr->send_task_num, schdlr->p->task_type, i, REQ_OUT, MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE);
                if(!p_comp_tasks(recv_tasks, no_task)) {
                    break;
                }
            }
        }
        if(q_get_counter(schdlr->p->q, &q_mutex) == 0 && i == schdlr->comm_size) {
            pthread_mutex_unlock(q_mutex);
            if((tmp = p_dec_task_cycles(schdlr->p, &p_mutex)) == 0) {
                pthread_mutex_unlock(p_mutex);
                break;
            } else {
                pthread_mutex_unlock(p_mutex);
                coeff = schdlr->p->task_cycles - tmp;
                p_generate_tasks(schdlr->p, schdlr->comm_size, schdlr->rank, &coeff);
            }
        } else {
            pthread_mutex_unlock(q_mutex);
            if(i != schdlr->comm_size) {
                j = 0;
                while(j < schdlr->send_task_num && !p_comp_tasks(recv_tasks + j, no_task)) {
                    p_task *t = malloc(sizeof(p_task));
                    p_copy_task(t, recv_tasks + j);
                    q_push(schdlr->p->q, t);
                    j++;
                }
            }
        }
    }

    p_free_task(no_task);
    req_type = REQ_END;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Send(&req_type, 1, MPI_INT, schdlr->rank, REQ_IN, MPI_COMM_WORLD);

}

void *simple_out_scheduler(void *arg) {

    scheduler *schdlr = (scheduler*)arg;
    int tmp, coeff, req_type;
    pthread_mutex_t *q_mutex, *p_mutex;

    while (1) {
        if(q_get_counter(schdlr->p->q, &q_mutex) == 0) {
            pthread_mutex_unlock(q_mutex);
            if((tmp = p_dec_task_cycles(schdlr->p, &p_mutex)) == 0) {
                pthread_mutex_unlock(p_mutex);
                break;
            } else {
                pthread_mutex_unlock(p_mutex);
                coeff = schdlr->p->task_cycles - tmp;
                p_generate_tasks(schdlr->p, schdlr->comm_size, schdlr->rank, &coeff);
            }
        } else {
            pthread_mutex_unlock(q_mutex);
        }
    }

    req_type = REQ_END;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Send(&req_type, 1, MPI_INT, schdlr->rank, REQ_IN, MPI_COMM_WORLD);
}