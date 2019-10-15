#include "ClientProcessing.h"

void *ClientProcessing::clientProcessing(void *clientProcessingObj) {
  bool error = false;
  ClientProcessing *obj = static_cast<ClientProcessing *>(clientProcessingObj);

  if (obj->getFileNameInfo() < 0) {
    std::perror("ClientProcessing::getFileNameInfo");
    error = true;
  }
  if (!error && obj->getFileSizeInfo() < 0) {
    std::perror("ClientProcessing::getFileSizeInfo");
    error = true;
  }
  if (!error && obj->getFile() < 0) {
    std::perror("ClientProcessing::getFile");
    error = true;
  }

  if (!error) {
    if (send(obj->socket, "OK", sizeof("OK"), 0) < 0) {
      std::perror("ClientProcessing::clientProcessing");
    }
    std::cout << obj->filename << ": succeed!" << std::endl;
  } else {
    if (send(obj->socket, "ER", sizeof("ER"), 0) < 0) {
      std::perror("ClientProcessing::clientProcessing");
    }
    std::cout << obj->filename << ": error!" << std::endl;
  }

  close(obj->socket);
  pthread_exit(nullptr);
}

ClientProcessing::ClientProcessing(int socket)
    : filename(MAX_FILE_NAME_LENGTH, '\0'), socket(socket) {}

int ClientProcessing::getFileNameInfo() {
  int name_len = 0;

  if (recv(socket, &name_len, sizeof(unsigned short), MSG_WAITALL) <= 0) {
    return -1;
  }
  if (name_len > MAX_FILE_NAME_LENGTH) {
    errno = ENAMETOOLONG;
    return -1;
  }

  if (recv(socket, (void *)filename.c_str(), name_len, MSG_WAITALL) <= 0) {
    return -1;
  }
  filename.resize(name_len);

  if (getNextFileName() < 0) {
    return -1;
  }

  return 0;
}

int ClientProcessing::getFileSizeInfo() {
  if (recv(socket, &filesize, sizeof(unsigned long long int), MSG_WAITALL) <=
      0) {
    return -1;
  }
  if (filesize > MAX_FILE_SIZE) {
    errno = EFBIG;
    return -1;
  }

  return 0;
}

int ClientProcessing::getFile() {
  char buffer[BUFSIZ];

  if ((file = creat(filename.c_str(),
                    S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) < 0) {
    return -1;
  }

  size_t len, curr_filesize = 0, prev_curr_filesize = 0;
  time_t prev_t = time(nullptr);
  time_t start_t = prev_t;
  time_t curr_t = 0;
  while (curr_filesize != filesize) {
    curr_t = time(nullptr);
    if (curr_t - prev_t >= INTERVAL) {
      printBytesPerSecond(": average speed: ", curr_filesize, curr_t - start_t);
      printBytesPerSecond(": instant speed: ",
                          curr_filesize - prev_curr_filesize, curr_t - prev_t);
      prev_t = curr_t;
      prev_curr_filesize = curr_filesize;
    }
    if ((len = recv(socket, buffer, BUFSIZ, 0)) <= 0) {
      break;
    }

    curr_filesize += len;

    if (write(file, buffer, len) != len) {
      break;
    }
  }
  printBytesPerSecond(": average speed: ", curr_filesize, curr_t - start_t);

  close(file);
  return (filesize == curr_filesize ? 0 : -1);
}

void ClientProcessing::printBytesPerSecond(const std::string &mark,
                                           unsigned long long int bytes,
                                           time_t seconds) {
  if (seconds == 0) {
    std::cout << filename << mark << bytes << " bytes ps." << std::endl;
    return;
  }
  std::cout << filename << mark << (bytes / seconds) << " bytes ps."
            << std::endl;
}

int ClientProcessing::getNextFileName() {
  DIR *uploads = opendir(".");
  if (!uploads) {
    return -1;
  }
  struct dirent *curr_file;
  int i = -1;
  while ((curr_file = readdir(uploads)) != nullptr) {
    auto curr_filename = std::string(curr_file->d_name);
    if (curr_filename.find(filename) == 0) {
      auto num = curr_filename.substr(filename.length());
      int tmp;
      if (num.length() == 0) {
        tmp = 0;
      } else {
        tmp = strtol(num.c_str(), nullptr, 10);
        tmp++;
      }
      if (tmp > i) {
        i = tmp;
      }
    }
  }

  if(i != -1) {
    filename += std::to_string(i);
  }
  return 0;
}

ClientProcessing::~ClientProcessing() { close(socket); }
