#include "pickupnamespace-p.h"
#include <stdio.h>
#include <stdlib.h>

struct node_rank {
  int node;
  int rank;
};

int compare_nodes(const void *a, const void *b) {
  struct node_rank arg1 = *(struct node_rank *)a;
  struct node_rank arg2 = *(struct node_rank *)b;

  return arg1.rank - arg2.rank;
}

char *pick_up_namespace(context_t ctx) {
  int flag;
  char *namespace = NULL;

  struct node_rank *ranking = malloc(sizeof(struct node_rank) * ctx->offset);

  for (int i = 0, flag = 0; i < ctx->offset; i++) {
    ranking[i].node = i;
    ranking[i].rank =
        ctx->nodes[i]->s - 2 * ctx->nodes[i]->amount * ctx->nodes[i]->rs;
    if (ranking[i].rank > ctx->nodes[i]->rs) {
      flag = 1;
    }
  }

  if (flag) {
    qsort(ranking, ctx->offset, sizeof(struct node_rank), compare_nodes);
    ctx->nodes[ranking[ctx->offset - 1].node]->amount++;
    namespace = ctx->nodes[ranking[ctx->offset - 1].node]->namespace;
  } else {
    fprintf(stderr, "Out of space\n");
  }

  free(ranking);
  return namespace;
}
