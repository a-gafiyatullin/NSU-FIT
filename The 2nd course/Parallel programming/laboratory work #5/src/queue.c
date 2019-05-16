//
// Created by xp10rd on 5/13/19.
//

#include "queue.h"

queue *q_init_queue() {
    queue *q;
    if((q = malloc(sizeof(queue))) == NULL) {
        return NULL;
    }
    if(pthread_mutex_init(&q->q_mutex, NULL) != 0) {
        free(q);
        return NULL;
    }
    q->q_counter = 0;
    q->q_first_node = NULL;
    q->q_last_node = NULL;

    return q;
}

int q_push(queue *q, void *data) {
    queue_node *node;
    if((node = malloc(sizeof(queue_node))) == NULL) {
        return -1;
    }
    node->q_data = data;
    node->q_next_node = NULL;

    pthread_mutex_lock(&q->q_mutex);
    if(q->q_first_node == NULL) {
        q->q_first_node = node;
        q->q_last_node = node;
    } else {
        q->q_last_node->q_next_node = node;
        q->q_last_node = node;
    }
    q->q_counter++;
    pthread_mutex_unlock(&q->q_mutex);

    return 0;
}

void *q_pop(queue *q) {
    pthread_mutex_lock(&q->q_mutex);
    if(q->q_first_node == NULL) {
        pthread_mutex_unlock(&q->q_mutex);
        return NULL;
    }
    queue_node *node = q->q_first_node;
    q->q_first_node = node->q_next_node;
    void *data = node->q_data;
    free(node);
    q->q_counter--;
    pthread_mutex_unlock(&q->q_mutex);

    return data;
}

int q_get_counter(queue *q, pthread_mutex_t **q_mutex) {
    pthread_mutex_lock(&q->q_mutex);
    *q_mutex = &q->q_mutex;
    return q->q_counter;
}

void q_free(queue *q, void (*u_free)(void*)) {
    pthread_mutex_lock(&q->q_mutex);
    queue_node *current = q->q_first_node;
    while(current != NULL) {
        u_free(current->q_data);
        current = current->q_next_node;
    }
    q->q_counter = 0;
    q->q_first_node = NULL;
    q->q_last_node = NULL;
    pthread_mutex_unlock(&q->q_mutex);
    free(q);
}