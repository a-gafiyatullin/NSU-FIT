//
// Created by xp10rd on 5/13/19.
//

#include "queue.h"

queue *q_init_queue() {
    queue *q;
    if((q = malloc(sizeof(queue))) == NULL) {
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

inline int q_get_counter(queue *q) { return q->q_counter; }