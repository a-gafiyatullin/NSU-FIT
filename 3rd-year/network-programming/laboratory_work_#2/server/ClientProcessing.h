#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

class ClientProcessing {
private:
  static const long long int MAX_FILE_SIZE = 1'099'511'627'776; // 1 Tb
  static const unsigned short MAX_FILE_NAME_LENGTH = 4096;      // byte
  static const time_t INTERVAL = 3;                             // 3 sec.

  int socket;
  int file;
  unsigned long long int filesize;
  std::string filename;

  int getFileNameInfo();
  int getFileSizeInfo();
  int getFile();
  int getNextFileName();

  void printBytesPerSecond(const std::string& mark, unsigned long long int bytes,
                           time_t seconds);

public:
  explicit ClientProcessing(int socket);

  static void *clientProcessing(void *clientProcessingObj);

  ~ClientProcessing();
};