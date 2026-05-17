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

// State tracking
static int16_t g_selectedIndex = 0;
static unsigned long g_lastSelectPress = 0;
static bool g_selectPressed = false;
static const unsigned long kDoubleClickWindow = 500; // milliseconds

void initializeButtons() {
  pinMode(board::kButtonUp, INPUT_PULLUP);
  pinMode(board::kButtonDown, INPUT_PULLUP);
  pinMode(board::kButtonSelect, INPUT_PULLUP);
}

void refreshDisplay(bool fullRefresh = false) {
  BabelTypesetterGFX* typesetter = g_babel.typesetter();

  g_display.clearBuffer(EPD_WHITE);
  
  if (fullRefresh) {
    g_ui.drawFullWithSelection(g_display.display(), typesetter, g_entries, kDefaultCategory, g_selectedIndex);
    g_display.presentFull(OPEN_BOOK_DISPLAY_MODE_QUICK);
  } else {
    // Partial refresh for selection highlight
    UiRect tableRect = g_ui.tableRect();
    int16_t contentY = tableRect.y + g_ui.dropdownRect().h + 10; // approximate header
    int16_t rowY = contentY + g_selectedIndex * 18; // 18 is rowHeight
    
    g_ui.drawFullWithSelection(g_display.display(), typesetter, g_entries, kDefaultCategory, g_selectedIndex);
    g_display.presentPartial(tableRect.x, rowY, tableRect.w, 18, OPEN_BOOK_DISPLAY_MODE_FASTPARTIAL);
  }
}

void setup() {
  bool displayReady = g_display.begin();
  bool sdReady = g_fs.begin();

  BabelTypesetterGFX* typesetter = nullptr;
  if (displayReady) {
    bool babelReady = g_babel.begin(g_display.display(), sdReady ? g_fs.sd() : nullptr);
    if (!babelReady) {
      if (Serial) {
        Serial.println("Babel initialization failed; using fallback text rendering.");
      }
    }
    typesetter = g_babel.typesetter();
  }

  if (sdReady) {
    g_entries = g_fs.scanRoot();
  }

  if (!displayReady) {
    return;
  }

  initializeButtons();

  g_display.clearBuffer(EPD_WHITE);
  g_ui.drawFullWithSelection(g_display.display(), typesetter, g_entries, kDefaultCategory, g_selectedIndex);
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
  const int kDebounceDelay = 50; // milliseconds
  static unsigned long lastButtonCheck = 0;
  unsigned long now = millis();

  if (now - lastButtonCheck < kDebounceDelay) {
    return;
  }
  lastButtonCheck = now;

  bool upPressed = digitalRead(board::kButtonUp) == LOW;
  bool downPressed = digitalRead(board::kButtonDown) == LOW;
  bool selectPressed = digitalRead(board::kButtonSelect) == LOW;

  // Handle UP button
  if (upPressed && g_selectedIndex > 0) {
    g_selectedIndex--;
    refreshDisplay(true);
    delay(100);
  }

  // Handle DOWN button
  if (downPressed && g_selectedIndex < (int16_t)g_entries.size() - 1) {
    g_selectedIndex++;
    refreshDisplay(true);
    delay(100);
  }

  // Handle SELECT/BACK button (double-click logic)
  if (selectPressed && !g_selectPressed) {
    unsigned long timeSinceLastPress = now - g_lastSelectPress;
    
    if (timeSinceLastPress < kDoubleClickWindow) {
      // Double click detected - go back
      if (g_fs.goBack()) {
        g_entries = g_fs.scanDirectory(g_fs.getCurrentPath());
        g_selectedIndex = 0;
        refreshDisplay(true);
      }
    } else {
      // Single click - open directory or file
      if (!g_entries.empty() && g_selectedIndex < (int16_t)g_entries.size()) {
        const FileEntry& entry = g_entries[g_selectedIndex];
        if (entry.isDir) {
          if (g_fs.openDirectory(entry.name.c_str())) {
            g_entries = g_fs.scanDirectory(g_fs.getCurrentPath());
            g_selectedIndex = 0;
            refreshDisplay(true);
          }
        }
        // For files, you could add file opening logic here later
      }
    }
    
    g_selectPressed = true;
    g_lastSelectPress = now;
    delay(100);
  }

  if (!selectPressed) {
    g_selectPressed = false;
  }
}
