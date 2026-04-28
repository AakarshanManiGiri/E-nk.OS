#pragma once

#include <stdint.h>

#ifndef OPENBOOK_PIN_UNASSIGNED
#define OPENBOOK_PIN_UNASSIGNED -1
#endif

#ifndef OPENBOOK_DISPLAY_WIDTH
#define OPENBOOK_DISPLAY_WIDTH 300
#endif
#ifndef OPENBOOK_DISPLAY_HEIGHT
#define OPENBOOK_DISPLAY_HEIGHT 400
#endif
#ifndef OPENBOOK_DISPLAY_ROTATION
#define OPENBOOK_DISPLAY_ROTATION 0
#endif

// Default pins derived from libros OpenBookDevice (ESP32-S3). Override via build flags.
#ifndef OPENBOOK_EPD_SCK
#define OPENBOOK_EPD_SCK 48
#endif
#ifndef OPENBOOK_EPD_MOSI
#define OPENBOOK_EPD_MOSI 47
#endif
#ifndef OPENBOOK_EPD_MISO
#define OPENBOOK_EPD_MISO OPENBOOK_PIN_UNASSIGNED
#endif
#ifndef OPENBOOK_EPD_CS
#define OPENBOOK_EPD_CS 35
#endif
#ifndef OPENBOOK_EPD_DC
#define OPENBOOK_EPD_DC 36
#endif
#ifndef OPENBOOK_EPD_RST
#define OPENBOOK_EPD_RST 38
#endif
#ifndef OPENBOOK_EPD_BUSY
#define OPENBOOK_EPD_BUSY 2
#endif
#ifndef OPENBOOK_EPD_SRAM_CS
#define OPENBOOK_EPD_SRAM_CS OPENBOOK_PIN_UNASSIGNED
#endif

#ifndef OPENBOOK_SD_SCK
#define OPENBOOK_SD_SCK 5
#endif
#ifndef OPENBOOK_SD_MOSI
#define OPENBOOK_SD_MOSI 4
#endif
#ifndef OPENBOOK_SD_MISO
#define OPENBOOK_SD_MISO 6
#endif
#ifndef OPENBOOK_SD_CS
#define OPENBOOK_SD_CS 37
#endif

#ifndef OPENBOOK_SD_MHZ
#define OPENBOOK_SD_MHZ 25
#endif

#ifndef OPENBOOK_BABEL_FROM_PARTITION
#define OPENBOOK_BABEL_FROM_PARTITION 1
#endif

#ifndef OPENBOOK_BABEL_PARTITION
#define OPENBOOK_BABEL_PARTITION "babel"
#endif

#ifndef OPENBOOK_BABEL_FILE
#define OPENBOOK_BABEL_FILE "/babel.bin"
#endif

namespace board {
constexpr int16_t kDisplayWidth = OPENBOOK_DISPLAY_WIDTH;
constexpr int16_t kDisplayHeight = OPENBOOK_DISPLAY_HEIGHT;
constexpr int16_t kDisplayRotation = OPENBOOK_DISPLAY_ROTATION;

constexpr int kEpdSck = OPENBOOK_EPD_SCK;
constexpr int kEpdMosi = OPENBOOK_EPD_MOSI;
constexpr int kEpdMiso = OPENBOOK_EPD_MISO;
constexpr int kEpdCs = OPENBOOK_EPD_CS;
constexpr int kEpdDc = OPENBOOK_EPD_DC;
constexpr int kEpdRst = OPENBOOK_EPD_RST;
constexpr int kEpdBusy = OPENBOOK_EPD_BUSY;
constexpr int kEpdSramCs = OPENBOOK_EPD_SRAM_CS;

constexpr int kSdSck = OPENBOOK_SD_SCK;
constexpr int kSdMosi = OPENBOOK_SD_MOSI;
constexpr int kSdMiso = OPENBOOK_SD_MISO;
constexpr int kSdCs = OPENBOOK_SD_CS;
constexpr int kSdMhz = OPENBOOK_SD_MHZ;

inline bool DisplayPinsValid() {
  return kEpdSck >= 0 && kEpdMosi >= 0 && kEpdCs >= 0 && kEpdDc >= 0 && kEpdRst >= 0;
}

inline bool SdPinsValid() {
  return kSdSck >= 0 && kSdMosi >= 0 && kSdMiso >= 0 && kSdCs >= 0;
}
} // namespace board
