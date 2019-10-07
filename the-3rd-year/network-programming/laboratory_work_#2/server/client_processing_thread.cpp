#include "client_processing_thread.h"

void *client_processing(void *client_socket) {
  auto socket = (size_t)client_socket;
  unsigned short name_len;
  int curr_file;
  unsigned long long int file_size;
  char buffer[BUFSIZ];

  if (recv(socket, &name_len, sizeof(unsigned short), MSG_WAITALL) <= 0) {
    std::perror("recv");
    pthread_exit((void *)(-1));
  }
  if (name_len > MAX_FILE_NAME_LENGTH) {
    std::cerr << "Wrong filename length!" << std::endl;
    pthread_exit((void *)(-1));
  }

  std::string filename(name_len + 1, '\0');
  if (recv(socket, (void *)filename.c_str(), name_len, MSG_WAITALL) <= 0) {
    std::perror("recv");
    pthread_exit((void *)(-1));
  }

  if (recv(socket, &file_size, sizeof(unsigned long long int), MSG_WAITALL) <=
      0) {
    std::perror("recv") pthread_exit((void *)(-1));
  }

  DIR *uploads = opendir(".");
  if (!uploads) {
    std::perror("opendir");
    pthread_exit((void *)(-1));
  }
  struct dirent *file;
  int i = 0;
  while ((file = readdir(uploads)) != nullptr) {
    if (filename == std::string(file->d_name)) {
      filename += std::to_string(i++);
    }
  }
  if ((curr_file = creat(filename.c_str(), S_IRWXU | S_IRGRP | S_IXGRP |
                                               S_IROTH | S_IXOTH)) < 0) {
    std::perror("creat");
    pthread_exit((void *)(-1));
  }

  while (1) {
  }
}