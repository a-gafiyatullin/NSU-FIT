//
// Created by xp10rd on 5/13/19.
//

#ifndef QUEUE_H
#define QUEUE_H

#include "stdlib.h"
#include <pthread.h>

typedef struct queue_node {
    struct queue_node *q_next_node;
    void *q_data;
} queue_node;


typedef struct queue {
    pthread_mutex_t q_mutex;
    queue_node *q_first_node;
    queue_node *q_last_node;
    int q_counter;
} queue;

queue *q_init_queue();

int q_push(queue *q, void *data);

void *q_pop(queue *q);

int q_get_counter(queue *q);

#endif //QUEUE_H
