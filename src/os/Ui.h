#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include <Adafruit_GFX.h>

#include "BabelTypesetterGFX.h"
#include "Filesystem.h"

struct UiRect {
  int16_t x = 0;
  int16_t y = 0;
  int16_t w = 0;
  int16_t h = 0;
};

class UiRenderer {
public:
  UiRenderer(int16_t width, int16_t height);

  UiRect dropdownRect() const;
  UiRect tableRect() const;

  void drawFull(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                const std::vector<FileEntry>& entries, const char* category);
  void drawFullWithSelection(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                             const std::vector<FileEntry>& entries, const char* category, 
                             int16_t selectedIndex);
  void drawDropdown(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                    const char* category, bool open);

private:
  void drawTable(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                 const std::vector<FileEntry>& entries);
  void drawHeaderRow(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                     int16_t y, int16_t col1X, int16_t col1W, int16_t col2X, int16_t col2W);
  void drawRow(Adafruit_GFX* display, BabelTypesetterGFX* typesetter, const FileEntry& entry,
               int16_t y, int16_t col1X, int16_t col1W, int16_t col2X, int16_t col2W, bool isSelected = false);

  void drawText(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                const char* text, int16_t x, int16_t y, int16_t w, int16_t h,
                bool bold, uint16_t color);
  void formatSize(uint64_t bytes, char* out, size_t outSize) const;

  int16_t width_;
  int16_t height_;
  int16_t padding_ = 8;
  int16_t dropdownHeight_ = 28;
  int16_t headerHeight_ = 18;
  int16_t rowHeight_ = 18;
  int16_t columnGap_ = 8;
};
