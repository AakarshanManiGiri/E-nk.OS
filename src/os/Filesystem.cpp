#include "Filesystem.h"

bool Filesystem::begin() {
  if (!board::SdPinsValid()) {
    return false;
  }

  sdSpi_.begin(board::kSdSck, board::kSdMiso, board::kSdMosi, board::kSdCs);
  sd_.reset(new SdFat(&sdSpi_));
  ready_ = sd_->begin(board::kSdCs, SD_SCK_MHZ(board::kSdMhz));
  return ready_;
}

std::vector<FileEntry> Filesystem::scanRoot() {
  std::vector<FileEntry> entries;
  if (!ready_ || !sd_) {
    return entries;
  }

  File root = sd_->open("/");
  if (!root) {
    return entries;
  }

  File entry = root.openNextFile();
  while (entry) {
    char name[128];
    name[0] = '\0';
    entry.getName(name, sizeof(name));

    if (name[0] != '\0' && name[0] != '.') {
      FileEntry info;
      info.name = name;
      info.size = entry.isDirectory() ? 0 : entry.size();
      info.isDir = entry.isDirectory();
      entries.push_back(info);
    }

    entry.close();
    entry = root.openNextFile();
  }

  root.close();
  return entries;
}

SdFat* Filesystem::sd() {
  return sd_.get();
}

bool Filesystem::ready() const {
  return ready_;
}
