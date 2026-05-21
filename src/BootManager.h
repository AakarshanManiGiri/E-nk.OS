#pragma once
#inlude <Arduino.h>

struct BootInfo {
    enum StorageType : uint8_t { NONE = 0, SD = 1, UNKNOWN = 0xFF };
    enum EinkType : uint8_t { EINK_NONE = 0, IL0323 = 1, UC8151 = 2, SSD1619 = 3 };
    enum InputType : uint8_t { INPUT_NONE = 0, GT911 = 1, FT5436 = 2 };

    StorageType storage = NONE;
    EinkType eink = EINK_NONE;
    InputType input = INPUT_NONE;
};

class BootManager {
public:
    static void begin();
    static const BootInfo& info();
private:
    static BootInfo _info;
    static void detectStorage();
    static void detectEink();
    static void detectInput();
};
