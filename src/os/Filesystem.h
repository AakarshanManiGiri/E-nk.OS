#pragma once

#include <memory>
#include <string>
#include <vector>
#include <SPI.h>
#include <SdFat.h>

#include "BoardConfig.h"

struct FileEntry {
  std::string name;
  uint64_t size = 0;
  bool isDir = false;
};

class Filesystem {
public:
  bool begin();
  std::vector<FileEntry> scanRoot();
  std::vector<FileEntry> scanDirectory(const char* path);
  bool openDirectory(const char* dirName);
  bool goBack();
  const char* getCurrentPath() const;
  SdFat* sd();
  bool ready() const;

private:
  SPIClass sdSpi_;
  std::unique_ptr<SdFat> sd_;
  std::string currentPath_;
  bool ready_ = false;
};
