#pragma once

#include <string>
#include <vector>
#include "Filesystem.h"

class FileReader {
public:
  FileReader(const char* path, SdFat* sd);
  ~FileReader();

  const char* getPage(int16_t page);
  int16_t getNumPages();

private:
  void paginate();

  File file_;
  std::vector<uint32_t> page_offsets_;
  char* page_buffer_ = nullptr;
  int16_t page_size_ = 400; // characters
};
