XX(EMPTY,        "Empty",        0x00, '\x20', 0x70)
XX(BOARD_EDGE,   "",             0x01, '\x20', 0xFF)
XX(MESSENGER,    "Messenger",    0x02, '\x20', 0xFF)
#if SUPER_ZZT
XX(MONITOR,      "Monitor",      0x03, '\x02', 0x1F)
#else /* !SUPER_ZZT */
XX(MONITOR,      "Monitor",      0x03, '\x20', 0x07)
#endif /* SUPER_ZZT */
XX(PLAYER,       "Player",       0x04, '\x02', 0x1F)
XX(AMMO,         "Ammo",         0x05, '\x84', 0x03)
#if !SUPER_ZZT
XX(TORCH,        "Torch",        0x06, '\x9D', 0x06)
#endif /* !SUPER_ZZT */
XX(GEM,          "Gem",          0x07, '\x04', 0xFF)
XX(KEY,          "Key",          0x08, '\x0C', 0xFF)
XX(DOOR,         "Door",         0x09, '\x0A', 0xFE)
XX(SCROLL,       "Scroll",       0x0A, '\xE8', 0x0F)
XX(PASSAGE,      "Passage",      0x0B, '\xF0', 0xFE)
XX(DUPLICATOR,   "Duplicator",   0x0C, '\xFA', 0x0F)
XX(BOMB,         "Bomb",         0x0D, '\x0B', 0xFF)
XX(ENERGIZER,    "Energizer",    0x0E, '\x7F', 0x05)
#if !SUPER_ZZT
XX(STAR,         "Star",         0x0F, '\x53', 0x0F)
#endif /* !SUPER_ZZT */
XX(CLOCKWISE,    "Clockwise",    0x10, '\x2F', 0xFF)
XX(COUNTER,      "Counter",      0x11, '\x5C', 0xFF)
#if !SUPER_ZZT
XX(BULLET,       "Bullet",       0x12, '\xF8', 0x0F)
#endif /* !SUPER_ZZT */
#if SUPER_ZZT
XX(LAVA,         "Lava",         0x13, '\x6F', 0x4E)
#else /* !SUPER_ZZT */
XX(WATER,        "Water",        0x13, '\xB0', 0xF9)
#endif /* SUPER_ZZT */
XX(FOREST,       "Forest",       0x14, '\xB0', 0x20)
XX(SOLID,        "Solid",        0x15, '\xDB', 0xFF)
XX(NORMAL,       "Normal",       0x16, '\xB2', 0xFF)
XX(BREAKABLE,    "Breakable",    0x17, '\xB1', 0xFF)
XX(BOULDER,      "Boulder",      0x18, '\xFE', 0xFF)
XX(SLIDER_NS,    "Slider (NS)",  0x19, '\x12', 0xFF)
XX(SLIDER_EW,    "Slider (EW)",  0x1A, '\x1D', 0xFF)
XX(FAKE,         "Fake",         0x1B, '\xB2', 0xFF)
XX(INVISIBLE,    "Invisible",    0x1C, '\xB0', 0xFF)

XX(BLINK_WALL,   "Blink wall",   0x1D, '\xCE', 0xFF)
XX(TRANSPORTER,  "Transporter",  0x1E, '\xC5', 0xFF)
XX(LINE,         "Line",         0x1F, '\xCE', 0xFF)
XX(RICOCHET,     "Ricochet",     0x20, '\x2A', 0x0A)
#if !SUPER_ZZT
XX(BLINK_RAY_H,  "",             0x21, '\xCD', 0xFF)
#endif /* !SUPER_ZZT */
#if SUPER_ZZT
XX(BEAR,         "Bear",         0x22, '\xEB', 0x02)
#else /* !SUPER_ZZT */
XX(BEAR,         "Bear",         0x22, '\x99', 0x06)
#endif /* SUPER_ZZT */
XX(RUFFIAN,      "Ruffian",      0x23, '\x05', 0x0D)
XX(OBJECT,       "Object",       0x24, '\x02', 0xFF)
XX(SLIME,        "Slime",        0x25, '\x2A', 0xFF)
#if !SUPER_ZZT
XX(SHARK,        "Shark",        0x26, '\x5E', 0x07)
#endif /* !SUPER_ZZT */
XX(SPINNING_GUN, "Spinning gun", 0x27, '\x18', 0xFF)
XX(PUSHER,       "Pusher",       0x28, '\x10', 0xFF)
XX(LION,         "Lion",         0x29, '\xEA', 0x0C)
XX(TIGER,        "Tiger",        0x2A, '\xE3', 0x0B)
#if !SUPER_ZZT
XX(BLINK_RAY_V,  "",             0x2B, '\xBA', 0xFF)
#endif /* !SUPER_ZZT */
XX(HEAD,         "Head",         0x2C, '\xE9', 0xFF)
XX(SEGMENT,      "Segment",      0x2D, '\x4F', 0xFF)
// 0x2E blank

#if SUPER_ZZT
XX(FLOOR,        "Floor",        0x2F, '\xB0', 0xFF)
XX(WATER_N,      "Water N",      0x30, '\x1E', 0x19)
XX(WATER_S,      "Water S",      0x31, '\x1F', 0x19)
XX(WATER_W,      "Water W",      0x32, '\x11', 0x19)
XX(WATER_E,      "Water E",      0x33, '\x10', 0x19)
// 0x34 through 0x3a blank

XX(ROTON,        "Roton",        0x3B, '\x94', 0x0D)
XX(DRAGON_PUP,   "Dragon pup",   0x3C, '\xED', 0x04)
XX(PAIRER,       "Pairer",       0x3D, '\xE5', 0x01)
XX(SPIDER,       "Spider",       0x3E, '\x0F', 0xFF)
XX(WEB,          "Web",          0x3F, '\xC5', 0xFF)
XX(STONE,        "Stone",        0x40, '\x5A', 0x0F)
// 0x41 through 0x44 blank

XX(BULLET,       "Bullet",       0x45, '\xF8', 0x0F)
XX(BLINK_RAY_H,  "",             0x46, '\xCD', 0xFF)
XX(BLINK_RAY_V,  "",             0x47, '\xBA', 0xFF)
XX(STAR,         "Star",         0x48, '\x53', 0x0F)
XX(TEXT_BLUE,    "",             0x49, '\x20', 0xFF)
XX(TEXT_GREEN,   "",             0x4A, '\x20', 0xFF)
XX(TEXT_CYAN,    "",             0x4B, '\x20', 0xFF)
XX(TEXT_RED,     "",             0x4C, '\x20', 0xFF)
XX(TEXT_PURPLE,  "",             0x4D, '\x20', 0xFF)
XX(TEXT_BROWN,   "",             0x4E, '\x20', 0xFF)
XX(TEXT_BLACK,   "",             0x4F, '\x20', 0xFF)
#else /* !SUPER_ZZT */
XX(TEXT_BLUE,    "",             0x2F, '\x20', 0xFF)
XX(TEXT_GREEN,   "",             0x30, '\x20', 0xFF)
XX(TEXT_CYAN,    "",             0x31, '\x20', 0xFF)
XX(TEXT_RED,     "",             0x32, '\x20', 0xFF)
XX(TEXT_PURPLE,  "",             0x33, '\x20', 0xFF)
XX(TEXT_BROWN,   "",             0x34, '\x20', 0xFF)
XX(TEXT_BLACK,   "",             0x35, '\x20', 0xFF)
#endif /* SUPER_ZZT */

