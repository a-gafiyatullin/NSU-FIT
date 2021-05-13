#include "FileSystemUtils.h"

std::string add_fs_delimeter_if_required(const std::string &file, const std::string &path) {
  std::string result = path;

  if(result.back() != '/') {
    result += "/";
  }

  return result + file;
}
