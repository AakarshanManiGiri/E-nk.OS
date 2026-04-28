#include "DisplayManager.h"

bool DisplayManager::begin() {
  if (!board::DisplayPinsValid()) {
    return false;
  }

  displaySpi_.begin(board::kEpdSck, board::kEpdMiso, board::kEpdMosi, board::kEpdCs);

  display_.reset(new OpenBook_IL0398(
      board::kDisplayWidth,
      board::kDisplayHeight,
      board::kEpdDc,
      board::kEpdRst,
      board::kEpdCs,
      board::kEpdSramCs,
      board::kEpdBusy,
      &displaySpi_));

  display_->begin(true);
  display_->setRotation(board::kDisplayRotation);
  return true;
}

OpenBook_IL0398* DisplayManager::display() {
  return display_.get();
}

void DisplayManager::clearBuffer(uint16_t color) {
  if (!display_) {
    return;
  }
  display_->clearBuffer();
  display_->fillScreen(color);
}

void DisplayManager::presentFull(OpenBookDisplayMode mode) {
  if (!display_) {
    return;
  }
  display_->setDisplayMode(mode);
  display_->display();
}

void DisplayManager::presentPartial(int16_t x, int16_t y, int16_t w, int16_t h, OpenBookDisplayMode mode) {
  if (!display_) {
    return;
  }
  display_->setDisplayMode(mode);
  display_->displayPartial(x, y, w, h);
}
