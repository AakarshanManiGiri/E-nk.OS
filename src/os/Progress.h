#pragma once

#include <SdFat.h>

class Progress {
public:
  Progress(SdFat* sd);
  void savePage(const char* bookPath, int16_t page);
  int16_t loadPage(const char* bookPath);

private:
  SdFat* sd_;
  const char* progress_file_ = "/.progress.json";
};
