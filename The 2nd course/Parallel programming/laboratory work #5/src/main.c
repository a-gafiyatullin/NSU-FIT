#include <stdio.h>
#include "scheduler.h"
#include "queue.h"
#include "task.h"

#define IN_SCHEDULER 0
#define OUT_SCHEDULER 1

void generate_tasks(queue *q, int size, int rank) {
    int coeff = TASKS_CYCLES - is_any_task_cycles();
    for(int i = 0; i < TASKS_NUM; i++) {
        q_push(q, gen_task(rank, size, &coeff));
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    srand(time(NULL));

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    tasks_init();
    queue *q = q_init_queue();
    int current_tasks_cycles = is_any_task_cycles();
    result rez;
    result_init(&rez);

    scheduler schdlr;
    schdlr.q = q;
    schdlr.rank = rank;
    schdlr.comm_size = size;
    pthread_t pthread[2];
    pthread_create(&pthread[IN_SCHEDULER], NULL, in_scheduler, &schdlr);
    pthread_create(&pthread[OUT_SCHEDULER], NULL, out_scheduler, &schdlr);

    generate_tasks(q, size, rank);
    while (is_any_task_cycles()) {
        if(current_tasks_cycles != is_any_task_cycles()) {
            generate_tasks(q, size, rank);
            current_tasks_cycles = is_any_task_cycles();
        } else {
            task *t = q_pop(q);
            if (t != NULL) {
                run_task(t, &rez);
            }
        }
    }

    pthread_join()
    print_result(&rez);
    return 0;
}