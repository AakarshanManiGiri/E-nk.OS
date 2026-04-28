#pragma once

#include <memory>
#include <Adafruit_GFX.h>
#include <SdFat.h>

#include "BabelTypesetterGFX.h"
#include "BoardConfig.h"

class BabelManager {
public:
  bool begin(Adafruit_GFX* display, SdFat* sd);
  BabelTypesetterGFX* typesetter();

private:
  std::unique_ptr<BabelTypesetterGFX> typesetter_;
};
