#include "FileReader.h"
#include <Arduino.h>

FileReader::FileReader(const char* path, SdFat* sd) {
  if (!sd) {
    return;
  }
  file_ = sd->open(path, O_READ);
  if (file_) {
    paginate();
    page_buffer_ = new char[page_size_ + 1];
  }
}

FileReader::~FileReader() {
  if (file_) {
    file_.close();
  }
  if (page_buffer_) {
    delete[] page_buffer_;
  }
}

void FileReader::paginate() {
  if (!file_) {
    return;
  }
  page_offsets_.push_back(0);
  while (file_.available()) {
    file_.seek(page_offsets_.back() + page_size_);
    if (file_.available()) {
      page_offsets_.push_back(file_.position());
    }
  }
  file_.seek(0);
}

const char* FileReader::getPage(int16_t page) {
  if (!file_ || !page_buffer_ || page < 0 || page >= getNumPages()) {
    return nullptr;
  }
  file_.seek(page_offsets_[page]);
  int bytes_read = file_.read(page_buffer_, page_size_);
  page_buffer_[bytes_read] = '\0';
  return page_buffer_;
}

int16_t FileReader::getNumPages() {
  return page_offsets_.size();
}
