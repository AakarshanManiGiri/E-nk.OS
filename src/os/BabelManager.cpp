#include "BabelManager.h"

bool BabelManager::begin(Adafruit_GFX* display, SdFat* sd) {
  if (!display) {
    return false;
  }

#if OPENBOOK_BABEL_FROM_PARTITION
  typesetter_.reset(new BabelTypesetterGFX(display, OPENBOOK_BABEL_PARTITION));
#else
#if defined(BOARD_REQUIRES_BABEL_FILE)
  if (!sd) {
    return false;
  }
  typesetter_.reset(new BabelTypesetterGFX(display, sd, const_cast<char*>(OPENBOOK_BABEL_FILE)));
#else
  (void)sd;
  return false;
#endif
#endif

  return typesetter_ && typesetter_->begin();
}

BabelTypesetterGFX* BabelManager::typesetter() {
  return typesetter_.get();
}
