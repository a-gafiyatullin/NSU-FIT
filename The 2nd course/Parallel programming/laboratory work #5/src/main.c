#include <stdio.h>
#include "scheduler.h"
#include "queue.h"
#include "task.h"

#define IN_SCHEDULER 0
#define OUT_SCHEDULER 1

#define TASK_CYCLES 200
#define IN_Q_TASK_NUM 7000
#define SEND_TASK_NUM (IN_Q_TASK_NUM / 100 * 5) //5% of tasks

int main(int argc, char *argv[]) {
    int type;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &type);
    if(type != MPI_THREAD_MULTIPLE) {
        fprintf(stderr, "Sorry, but this program is THREAD MULTIPLE!\n");
        MPI_Finalize();
        return -1;
    }

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    pthread_mutex_t *p_mutex;
    p_problem *p = p_init(TASK_CYCLES, IN_Q_TASK_NUM);
    if(p == NULL) {
        fprintf(stderr, "p_init: can't initialize tasks!\n");
        MPI_Finalize();
        return -1;
    }
    p_result *global_rez;
    int coeff = p->task_cycles - p_is_any_task_cycles(p, &p_mutex);
    pthread_mutex_unlock(p_mutex);

    scheduler schdlr;
    schdlr.p = p;
    schdlr.rank = rank;
    schdlr.comm_size = size;
    schdlr.send_task_num = (SEND_TASK_NUM == 0 ? 1 : SEND_TASK_NUM);
    pthread_t pthread[2];
    double start, end;
    p_generate_tasks(p, size, rank, &coeff);

    start = MPI_Wtime();
    pthread_create(&pthread[IN_SCHEDULER], NULL, in_scheduler, &schdlr);
    pthread_create(&pthread[OUT_SCHEDULER], NULL, out_scheduler, &schdlr);
    while (1) {
        if(p_is_any_task_cycles(p, &p_mutex) == 0) {
            break;
        }
        pthread_mutex_unlock(p_mutex);
        p_task *t = q_pop(p->q);
        if (t != NULL) {
            p_run_task(t, p);
            p_free_task(t);
        }
    }

    pthread_mutex_unlock(p_mutex);
    pthread_join(pthread[OUT_SCHEDULER], NULL);
    pthread_join(pthread[IN_SCHEDULER], NULL);
    global_rez = p_reduce_result(MPI_COMM_WORLD, p);
    end = MPI_Wtime();
    p_print_result(p->rez);

    if(rank == 0) {
        printf("global p_result: ");
        p_print_result(global_rez);
        printf("Total time: %lf\n", end - start);
    }
    p_free(p);
    MPI_Finalize();
    return 0;
}