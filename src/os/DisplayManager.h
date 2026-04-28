#pragma once

#include <memory>
#include <SPI.h>

#include "OpenBook_EPD.h"
#include "OpenBook_IL0398.h"
#include "BoardConfig.h"

class DisplayManager {
public:
  bool begin();
  OpenBook_IL0398* display();
  void clearBuffer(uint16_t color);
  void presentFull(OpenBookDisplayMode mode);
  void presentPartial(int16_t x, int16_t y, int16_t w, int16_t h, OpenBookDisplayMode mode);

private:
  SPIClass displaySpi_;
  std::unique_ptr<OpenBook_IL0398> display_;
};
