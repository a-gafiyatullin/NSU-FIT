#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef PICKUPNAMESPACE_H
#define PICKUPNAMESPACE_H

#define MAX_NUMBER_OF_NODES 100 /* кол-во узлов */

typedef struct node {
  char *namespace;
  size_t amount;
  size_t rs;
  size_t s;
  size_t m;
  size_t c;
} * node_t;

typedef struct context {
  size_t max;
  size_t offset;
  node_t *nodes;
} * context_t;

context_t context_create(size_t max);

int context_set_node(context_t ctx, const char *name, size_t rs, size_t s,
                     size_t m, size_t c);

int context_destroy();

char *pick_up_namespace(context_t ctx);

#endif