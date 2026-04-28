#include <Arduino.h>

#include "Adafruit_EPD.h"
#include "OpenBook_EPD.h"

#include "os/BoardConfig.h"
#include "os/BabelManager.h"
#include "os/DisplayManager.h"
#include "os/Filesystem.h"
#include "os/Ui.h"

static DisplayManager g_display;
static Filesystem g_fs;
static BabelManager g_babel;
static UiRenderer g_ui(board::kDisplayWidth, board::kDisplayHeight);
static std::vector<FileEntry> g_entries;

static const char* kDefaultCategory = "All Files";

void setup() {
  bool displayReady = g_display.begin();
  bool sdReady = g_fs.begin();

  BabelTypesetterGFX* typesetter = nullptr;
  if (displayReady) {
    g_babel.begin(g_display.display(), sdReady ? g_fs.sd() : nullptr);
    typesetter = g_babel.typesetter();
  }

  if (sdReady) {
    g_entries = g_fs.scanRoot();
  }

  if (!displayReady) {
    return;
  }

  g_display.clearBuffer(EPD_WHITE);
  g_ui.drawFull(g_display.display(), typesetter, g_entries, kDefaultCategory);
  g_display.presentFull(OPEN_BOOK_DISPLAY_MODE_QUICK);

  UiRect menuRect = g_ui.dropdownRect();
  delay(120);
  g_ui.drawDropdown(g_display.display(), typesetter, kDefaultCategory, true);
  g_display.presentPartial(menuRect.x, menuRect.y, menuRect.w, menuRect.h, OPEN_BOOK_DISPLAY_MODE_FASTPARTIAL);
  delay(120);
  g_ui.drawDropdown(g_display.display(), typesetter, kDefaultCategory, false);
  g_display.presentPartial(menuRect.x, menuRect.y, menuRect.w, menuRect.h, OPEN_BOOK_DISPLAY_MODE_FASTPARTIAL);
}

void loop() {
}
