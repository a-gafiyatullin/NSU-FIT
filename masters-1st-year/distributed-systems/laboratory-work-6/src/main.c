#include "pickupnamespace-p.h"
#include <stdio.h>

#define MAX 20
#define NNODES 5
#define NREQ 13

int main() {
  context_t ctx = context_create(MAX);

  for (int i = 0; i < NNODES; i++) {
    char buf[32];
    buf[0] = '\0';
    sprintf(buf, "NODE_%02d", i);
    context_set_node(ctx, buf, 10, 100, 1, 1);
  }

  printf("created context (max:%d)\n", MAX);
  printf("added %d nodes\n", NNODES);
  printf("planned %d request of namespace\n", NREQ);

  for (int i = 0; i < NREQ; i++) {
    pick_up_namespace(ctx);
  }

  printf("Result:\n");
  for (int i = 0; i < ctx->offset; i++) {
    printf("\tindex:%d\t\tnamespace:%s\tamount:%zu\t\n", i,
           ctx->nodes[i]->namespace, ctx->nodes[i]->amount);
  }

  context_destroy(ctx);

  return 0;
}
