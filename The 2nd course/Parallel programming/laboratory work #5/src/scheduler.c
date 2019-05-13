//
// Created by xp10rd on 5/13/19.
//

#include "scheduler.h"

void *in_scheduler(void *arg) {

    scheduler *schdlr = (scheduler*)arg;
    int req_type;
    MPI_Status status;
    task *t, no_task = get_no_tasks_structure();

    while (1) {
        MPI_Recv(&req_type, 1, MPI_INT, MPI_ANY_SOURCE, REQ_IN, MPI_COMM_WORLD, &status);
        if(req_type == REQ_TASK) {
            if(q_get_counter(schdlr->q) <= MINIMAL_IN_QUEUE_TASKS_NUM) {
                t = NULL;
            } else {
                t = q_pop(schdlr->q);
            }
            if(t == NULL) {
                t = &no_task;
            }
            MPI_Send(t, 1, task_type, status.MPI_SOURCE, REQ_OUT, MPI_COMM_WORLD);
        } else if(req_type == REQ_END) {
            break;
        }
    }

}

void *out_scheduler(void *arg) {

    scheduler *schdlr = (scheduler*)arg;
    int i;
    int req_type;
    task no_task = get_no_tasks_structure();

    while (1) {
        if(q_get_counter(schdlr->q) <= MINIMAL_IN_QUEUE_TASKS_NUM) {
            req_type = REQ_TASK;
            task *t = malloc(sizeof(task));
            for(i = 0; i < schdlr->comm_size; i++) {
                if(i != schdlr->rank) {
                    MPI_Send(&req_type, 1, MPI_INT, i, REQ_IN, MPI_COMM_WORLD);
                    MPI_Recv(t, 1, task_type, i, REQ_OUT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    if(!compare_tasks(t, &no_task)) {
                        break;
                    }
                }
            }
            if(i == schdlr->comm_size) {
                if(decrease_task_cycles() == 0) {
                    req_type = REQ_END;
                    MPI_Send(&req_type, 1, MPI_INT, schdlr->rank, REQ_IN, MPI_COMM_WORLD);
                    break;
                }
            }
            q_push(schdlr->q, t);
        }
    }

}