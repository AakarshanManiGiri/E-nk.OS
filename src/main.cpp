#include <Arduino.h>
#include <vector>
#include <string>
#include "memory"

#include "Adafruit_EPD.h"
#include "OpenBook_EPD.h"

#include "os/BoardConfig.h"
#include "os/BabelManager.h"
#include "os/DisplayManager.h"
#include "os/Filesystem.h"
#include "os/Ui.h"
#include "os/FileReader.h"
#include "os/Progress.h"

static DisplayManager g_display;
static Filesystem g_fs;
static BabelManager g_babel;
static UiRenderer g_ui(board::kDisplayWidth, board::kDisplayHeight);
static std::vector<FileEntry> g_entries;
static std::unique_ptr<FileReader> g_fileReader;
static std::unique_ptr<Progress> g_progress;

static const char* kDefaultCategory = "All Files";
static const std::vector<std::string> kFileContextMenuItems = {"Open", "Rename", "Delete", "Cancel"};
static const std::vector<std::string> kDirContextMenuItems = {"Rename", "Delete", "Cancel"};
static std::vector<std::string> g_currentContextMenuItems;

enum UiState {
    STATE_BROWSING,
    STATE_CONTEXT_MENU,
    STATE_READING,
};

// State tracking
static UiState g_uiState = STATE_BROWSING;
static int16_t g_selectedIndex = 0;
static int16_t g_contextMenuSelection = 0;
static int16_t g_currentPage = 0;
static int16_t g_lastPage = 0;
static unsigned long g_lastSelectPress = 0;
static bool g_selectPressed = false;
static unsigned long g_selectPressTime = 0;
static bool g_longPressTriggered = false;
static const unsigned long kDoubleClickWindow = 500; // milliseconds
static const unsigned long kLongPressDuration = 500; // milliseconds

void initializeButtons() {
  pinMode(board::kButtonUp, INPUT_PULLUP);
  pinMode(board::kButtonDown, INPUT_PULLUP);
  pinMode(board::kButtonSelect, INPUT_PULLUP);
}

void refreshDisplay(bool fullRefresh = false) {
  BabelTypesetterGFX* typesetter = g_babel.typesetter();

  g_display.clearBuffer(EPD_WHITE);

  switch (g_uiState) {
    case STATE_BROWSING:
      g_ui.drawFullWithSelection(g_display.display(), typesetter, g_entries, kDefaultCategory, g_selectedIndex);
      break;
    case STATE_CONTEXT_MENU:
      g_ui.drawFullWithSelection(g_display.display(), typesetter, g_entries, kDefaultCategory, g_selectedIndex);
      g_ui.drawContextMenu(g_display.display(), typesetter, g_currentContextMenuItems, g_contextMenuSelection, 50, 50);
      break;
    case STATE_READING:
      if (g_fileReader) {
        const char* pageText = g_fileReader->getPage(g_currentPage);
        g_ui.drawReaderView(g_display.display(), typesetter, pageText, g_currentPage + 1, g_fileReader->getNumPages());
      }
      break;
  }
  
  if (fullRefresh) {
    g_display.presentFull(OPEN_BOOK_DISPLAY_MODE_QUICK);
  } else {
    g_display.presentPartial(0, 0, board::kDisplayWidth, board::kDisplayHeight, OPEN_BOOK_DISPLAY_MODE_FASTPARTIAL);
  }
}

void setup() {
  bool displayReady = g_display.begin();
  bool sdReady = g_fs.begin();
  g_progress = std::make_unique<Progress>(g_fs.sd());

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

  switch (g_uiState) {
    case STATE_BROWSING:
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

      // Handle SELECT button (short, long, and double-click)
      if (selectPressed) {
        if (!g_selectPressed) {
          // First press
          g_selectPressed = true;
          g_longPressTriggered = false;
          g_selectPressTime = now;
        } else if (!g_longPressTriggered && (now - g_selectPressTime > kLongPressDuration)) {
          // Long press detected
          g_longPressTriggered = true;
          g_uiState = STATE_CONTEXT_MENU;
          g_contextMenuSelection = 0;
          if (!g_entries.empty() && g_selectedIndex < (int16_t)g_entries.size()) {
            if (g_entries[g_selectedIndex].isDir) {
              g_currentContextMenuItems = kDirContextMenuItems;
            } else {
              g_currentContextMenuItems = kFileContextMenuItems;
            }
          }
          refreshDisplay(true);
        }
      } else {
        if (g_selectPressed && !g_longPressTriggered) {
          // Released from a short press
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
          g_lastSelectPress = now;
        }
        g_selectPressed = false;
      }
      break;

    case STATE_CONTEXT_MENU:
      if (upPressed && g_contextMenuSelection > 0) {
        g_contextMenuSelection--;
        refreshDisplay(false);
        delay(100);
      }
      if (downPressed && g_contextMenuSelection < (int16_t)g_currentContextMenuItems.size() - 1) {
        g_contextMenuSelection++;
        refreshDisplay(false);
        delay(100);
      }
      if (selectPressed && !g_selectPressed) {
        std::string action = g_currentContextMenuItems[g_contextMenuSelection];
        if (action == "Open") {
            const FileEntry& entry = g_entries[g_selectedIndex];
            std::string path = g_fs.getCurrentPath();
            if (path.back() != '/') {
                path += "/";
            }
            path += entry.name;
            g_fileReader = std::make_unique<FileReader>(path.c_str(), g_fs.sd());
            g_lastPage = g_progress->loadPage(path.c_str());
            g_currentPage = g_lastPage;
            g_uiState = STATE_READING;
        }
        else if (action == "Delete") {
          const FileEntry& entry = g_entries[g_selectedIndex];
          std::string path = g_fs.getCurrentPath();
          if (path.back() != '/') {
            path += "/";
          }
          path += entry.name;
          g_fs.deleteFile(path.c_str());
          g_entries = g_fs.scanDirectory(g_fs.getCurrentPath());
          g_selectedIndex = 0;
        } else if (action == "Cancel") {
          // do nothing
        }
        g_uiState = STATE_BROWSING;
        refreshDisplay(true);
        g_selectPressed = true;
        delay(100);
      }
      if (!selectPressed) {
        g_selectPressed = false;
      }
      break;
    
    case STATE_READING:
        if (upPressed && downPressed) {
            g_currentPage = g_lastPage;
            refreshDisplay(true);
            delay(100);
        } else if (upPressed && g_currentPage > 0) {
            g_currentPage--;
            refreshDisplay(true);
            delay(100);
        } else if (downPressed && g_fileReader && g_currentPage < g_fileReader->getNumPages() - 1) {
            g_currentPage++;
            refreshDisplay(true);
            delay(100);
        } else if (selectPressed && !g_selectPressed) {
            unsigned long timeSinceLastPress = now - g_lastSelectPress;
            if (timeSinceLastPress < kDoubleClickWindow) {
            // Double click detected - go back
            if (g_fileReader) {
                const FileEntry& entry = g_entries[g_selectedIndex];
                std::string path = g_fs.getCurrentPath();
                if (path.back() != '/') {
                    path += "/";
                }
                path += entry.name;
                g_progress->savePage(path.c_str(), g_currentPage);
            }
            g_fileReader.reset();
            g_uiState = STATE_BROWSING;
            refreshDisplay(true);
            }
            g_lastSelectPress = now;
            g_selectPressed = true;
            delay(100);
        }
        if (!selectPressed) {
            g_selectPressed = false;
        }
        break;
  }
}
