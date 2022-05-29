#include "pickupnamespace-p.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

context_t context_create(size_t max) {
  int i;
  context_t ctx = 0;

  ctx = (context_t)malloc(sizeof(*ctx));

  if (max && max < MAX_NUMBER_OF_NODES) {
    ctx->nodes = (node_t *)malloc(max * sizeof(struct node));
    ctx->max = max;
  } else {
    ctx->nodes = (node_t *)malloc(MAX_NUMBER_OF_NODES * sizeof(struct node));
    ctx->max = MAX_NUMBER_OF_NODES;
  }

  for (i = 0; i < ctx->max; i++)
    ctx->nodes[i] = NULL;

  ctx->offset = 0;

  return ctx;
}

int context_set_node(context_t ctx, const char *namespace, size_t rs, size_t s,
                     size_t m, size_t c) {
  int rc = 0;

  if (!ctx)
    return -1;

  if (ctx->offset < ctx->max) {
    node_t node;

    node = (node_t)malloc(sizeof(struct node));
    node->namespace = (char *)strdup(namespace);
    node->rs = rs;
    node->s = s;
    node->m = m;
    node->c = c;
    node->amount = 0;

    ctx->nodes[ctx->offset++] = node;
  } else {
    rc = -1;
  }

  return rc;
}

int context_destroy(context_t ctx) {
  int i;

  if (!ctx)
    return -1;

  for (i = 0; i < ctx->offset; i++) {
    free((char *)ctx->nodes[i]->namespace);
    free((node_t)ctx->nodes[i]);
  };

  free((node_t *)ctx->nodes);
  free((context_t)ctx);

  return 0;
}
