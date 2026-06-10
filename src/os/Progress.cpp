#include "Progress.h"
#include <ArduinoJson.h> // Requires ArduinoJson library by Benoît Blanchon

Progress::Progress(SdFat* sd) : sd_(sd) {}

void Progress::savePage(const char* bookPath, int16_t page) {
  if (!sd_) return;

  DynamicJsonDocument doc(1024);
  File file = sd_->open(progress_file_, O_READ);
  if (file) {
    deserializeJson(doc, file);
    file.close();
  }

  doc[bookPath] = page;

  file = sd_->open(progress_file_, O_WRITE | O_CREATE | O_TRUNC);
  if (file) {
    serializeJson(doc, file);
    file.close();
  }
}

int16_t Progress::loadPage(const char* bookPath) {
  if (!sd_) return 0;

  File file = sd_->open(progress_file_, O_READ);
  if (!file) {
    return 0;
  }

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    return 0;
  }

  return doc[bookPath] | 0;
}
