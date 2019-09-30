#include "client_processing_thread.h"

void *client_processing(void *client_socket) {
  size_t socket = (size_t)client_socket;
  unsigned short name_len;

  if (recv(socket, &name_len, sizeof(unsigned short), MSG_WAITALL) <= 0) {
    std::perror("recv");
  }

  while (1) {
  }
}