#include "Ui.h"

#include <algorithm>
#include <stdio.h>

#include "Adafruit_EPD.h"

UiRenderer::UiRenderer(int16_t width, int16_t height) : width_(width), height_(height) {
}

UiRect UiRenderer::dropdownRect() const {
  UiRect rect;
  rect.x = padding_;
  rect.y = padding_;
  rect.w = width_ - padding_ * 2;
  rect.h = dropdownHeight_;
  return rect;
}

UiRect UiRenderer::tableRect() const {
  UiRect rect;
  UiRect menu = dropdownRect();
  rect.x = padding_;
  rect.y = menu.y + menu.h + padding_;
  rect.w = width_ - padding_ * 2;
  rect.h = height_ - rect.y - padding_;
  return rect;
}

void UiRenderer::drawFull(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                          const std::vector<FileEntry>& entries, const char* category) {
  if (!display) {
    return;
  }

  drawDropdown(display, typesetter, category, false);
  drawTable(display, typesetter, entries);
}

void UiRenderer::drawDropdown(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                              const char* category, bool open) {
  if (!display) {
    return;
  }

  UiRect rect = dropdownRect();
  uint16_t bg = open ? EPD_DARK : EPD_LIGHT;
  uint16_t fg = open ? EPD_WHITE : EPD_BLACK;

  display->fillRect(rect.x, rect.y, rect.w, rect.h, bg);
  display->drawRect(rect.x, rect.y, rect.w, rect.h, EPD_BLACK);

  int16_t arrowX = rect.x + rect.w - 12;
  int16_t arrowY = rect.y + rect.h / 2;
  if (open) {
    display->fillTriangle(arrowX - 4, arrowY + 2, arrowX + 4, arrowY + 2, arrowX, arrowY - 3, fg);
  } else {
    display->fillTriangle(arrowX - 4, arrowY - 3, arrowX + 4, arrowY - 3, arrowX, arrowY + 2, fg);
  }

  drawText(display, typesetter, category ? category : "All Files",
           rect.x + 8, rect.y, rect.w - 24, rect.h, true, fg);
}

void UiRenderer::drawTable(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                           const std::vector<FileEntry>& entries) {
  if (!display) {
    return;
  }

  UiRect rect = tableRect();
  display->drawRect(rect.x, rect.y, rect.w, rect.h, EPD_BLACK);

  int16_t innerX = rect.x + 4;
  int16_t innerW = rect.w - 8;
  int16_t col2W = 72;
  int16_t col1W = innerW - columnGap_ - col2W;
  int16_t col1X = innerX;
  int16_t col2X = innerX + col1W + columnGap_;

  drawHeaderRow(display, typesetter, rect.y, col1X, col1W, col2X, col2W);

  int16_t contentY = rect.y + headerHeight_;
  int16_t contentH = rect.h - headerHeight_;
  int16_t maxRows = contentH / rowHeight_;

  if (entries.empty()) {
    drawText(display, typesetter, "No files found", rect.x + 8, contentY + 2,
             rect.w - 16, rowHeight_, false, EPD_BLACK);
    return;
  }

  int16_t row = 0;
  for (const auto& entry : entries) {
    if (row >= maxRows) {
      break;
    }

    int16_t rowY = contentY + row * rowHeight_;
    drawRow(display, typesetter, entry, rowY, col1X, col1W, col2X, col2W);
    display->drawFastHLine(rect.x, rowY + rowHeight_ - 1, rect.w, EPD_LIGHT);
    row++;
  }
}

void UiRenderer::drawHeaderRow(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                               int16_t y, int16_t col1X, int16_t col1W, int16_t col2X, int16_t col2W) {
  if (!display) {
    return;
  }

  display->fillRect(col1X - 4, y + 1, col1W + col2W + columnGap_ + 8, headerHeight_ - 1, EPD_LIGHT);
  drawText(display, typesetter, "Filename", col1X, y, col1W, headerHeight_, true, EPD_BLACK);
  drawText(display, typesetter, "Size", col2X, y, col2W, headerHeight_, true, EPD_BLACK);
}

void UiRenderer::drawRow(Adafruit_GFX* display, BabelTypesetterGFX* typesetter, const FileEntry& entry,
                         int16_t y, int16_t col1X, int16_t col1W, int16_t col2X, int16_t col2W) {
  if (!display) {
    return;
  }

  std::string label = entry.name;
  if (entry.isDir && !label.empty() && label.back() != '/') {
    label += "/";
  }

  char sizeText[16];
  if (entry.isDir) {
    snprintf(sizeText, sizeof(sizeText), "%s", "DIR");
  } else {
    formatSize(entry.size, sizeText, sizeof(sizeText));
  }

  drawText(display, typesetter, label.c_str(), col1X, y, col1W, rowHeight_, false, EPD_BLACK);
  drawText(display, typesetter, sizeText, col2X, y, col2W, rowHeight_, false, EPD_BLACK);
}

void UiRenderer::drawText(Adafruit_GFX* display, BabelTypesetterGFX* typesetter,
                          const char* text, int16_t x, int16_t y, int16_t w, int16_t h,
                          bool bold, uint16_t color) {
  if (!display || !text) {
    return;
  }

  int16_t fontHeight = 16;
  if (typesetter && typesetter->getBabel()) {
    fontHeight = typesetter->getBabel()->getHeight();
  }

  int16_t textY = y + (h - fontHeight) / 2;

  if (typesetter && typesetter->getBabel()) {
    typesetter->setLayoutArea(x, y, w, h);
    typesetter->setCursor(x, textY);
    typesetter->setWordWrap(false);
    typesetter->setBold(bold);
    typesetter->setItalic(false);
    typesetter->setTextSize(1);
    typesetter->setLineSpacing(0);
    typesetter->setParagraphSpacing(0);
    typesetter->setTextColor(color);
    typesetter->print(text);
  } else {
    display->setTextColor(color);
    display->setTextSize(1);
    display->setCursor(x, textY);
    display->print(text);
  }
}

void UiRenderer::formatSize(uint64_t bytes, char* out, size_t outSize) const {
  if (!out || outSize == 0) {
    return;
  }

  if (bytes < 1024ULL) {
    snprintf(out, outSize, "%llu B", static_cast<unsigned long long>(bytes));
  } else if (bytes < 1024ULL * 1024ULL) {
    unsigned long long kb = (bytes + 1023ULL) / 1024ULL;
    snprintf(out, outSize, "%llu KB", kb);
  } else if (bytes < 1024ULL * 1024ULL * 1024ULL) {
    unsigned long long mb = (bytes + 1024ULL * 1024ULL - 1ULL) / (1024ULL * 1024ULL);
    snprintf(out, outSize, "%llu MB", mb);
  } else {
    unsigned long long gb = (bytes + 1024ULL * 1024ULL * 1024ULL - 1ULL) / (1024ULL * 1024ULL * 1024ULL);
    snprintf(out, outSize, "%llu GB", gb);
  }
}
