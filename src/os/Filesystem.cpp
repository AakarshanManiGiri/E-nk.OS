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
  currentPath_ = "/";
  return scanDirectory("/");
}

std::vector<FileEntry> Filesystem::scanDirectory(const char* path) {
  std::vector<FileEntry> entries;
  if (!ready_ || !sd_) {
    return entries;
  }

  File root = sd_->open(path);
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

bool Filesystem::openDirectory(const char* dirName) {
  if (!dirName || dirName[0] == '\0') {
    return false;
  }

  std::string newPath = currentPath_;
  if (newPath.back() != '/') {
    newPath += "/";
  }
  newPath += dirName;

  File dir = sd_->open(newPath.c_str());
  if (!dir || !dir.isDirectory()) {
    dir.close();
    return false;
  }
  dir.close();

  currentPath_ = newPath;
  return true;
}

bool Filesystem::goBack() {
  if (currentPath_ == "/") {
    return false;
  }

  size_t lastSlash = currentPath_.rfind('/');
  if (lastSlash == 0) {
    currentPath_ = "/";
  } else {
    currentPath_ = currentPath_.substr(0, lastSlash);
  }
  return true;
}

const char* Filesystem::getCurrentPath() const {
  return currentPath_.c_str();
}

SdFat* Filesystem::sd() {
  return sd_.get();
}

bool Filesystem::ready() const {
  return ready_;
}
