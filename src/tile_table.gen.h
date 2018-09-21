XX(EMPTY,        "Empty",        0x00, '\x20', 0x70, -1)
XX(BOARD_EDGE,   "",             0x01, '\x20', 0xFF, -1)
XX(MESSENGER,    "Messenger",    0x02, '\x20', 0xFF, -1)
#if SUPER_ZZT
XX(MONITOR,      "Monitor",      0x03, '\x02', 0x1F,  1)
#else /* !SUPER_ZZT */
XX(MONITOR,      "Monitor",      0x03, '\x20', 0x07,  1)
#endif /* SUPER_ZZT */
XX(PLAYER,       "Player",       0x04, '\x02', 0x1F,  1)
XX(AMMO,         "Ammo",         0x05, '\x84', 0x03, -1)
#if !SUPER_ZZT
XX(TORCH,        "Torch",        0x06, '\x9D', 0x06, -1)
#endif /* !SUPER_ZZT */
XX(GEM,          "Gem",          0x07, '\x04', 0xFF, -1)
XX(KEY,          "Key",          0x08, '\x0C', 0xFF, -1)
XX(DOOR,         "Door",         0x09, '\x0A', 0xFE, -1)
XX(SCROLL,       "Scroll",       0x0A, '\xE8', 0x0F,  1)
XX(PASSAGE,      "Passage",      0x0B, '\xF0', 0xFE,  0)
XX(DUPLICATOR,   "Duplicator",   0x0C, '\xFA', 0x0F,  2)
XX(BOMB,         "Bomb",         0x0D, '\x0B', 0xFF,  6)
XX(ENERGIZER,    "Energizer",    0x0E, '\x7F', 0x05, -1)
#if !SUPER_ZZT
XX(STAR,         "Star",         0x0F, '\x53', 0x0F,  1)
#endif /* !SUPER_ZZT */
XX(CLOCKWISE,    "Clockwise",    0x10, '\x2F', 0xFF,  3)
XX(COUNTER,      "Counter",      0x11, '\x5C', 0xFF,  2)
#if !SUPER_ZZT
XX(BULLET,       "Bullet",       0x12, '\xF8', 0x0F,  1)
#endif /* !SUPER_ZZT */
#if SUPER_ZZT
XX(LAVA,         "Lava",         0x13, '\x6F', 0x4E, -1)
#else /* !SUPER_ZZT */
XX(WATER,        "Water",        0x13, '\xB0', 0xF9, -1)
#endif /* SUPER_ZZT */
XX(FOREST,       "Forest",       0x14, '\xB0', 0x20, -1)
XX(SOLID,        "Solid",        0x15, '\xDB', 0xFF, -1)
XX(NORMAL,       "Normal",       0x16, '\xB2', 0xFF, -1)
XX(BREAKABLE,    "Breakable",    0x17, '\xB1', 0xFF, -1)
XX(BOULDER,      "Boulder",      0x18, '\xFE', 0xFF, -1)
XX(SLIDER_NS,    "Slider (NS)",  0x19, '\x12', 0xFF, -1)
XX(SLIDER_EW,    "Slider (EW)",  0x1A, '\x1D', 0xFF, -1)
XX(FAKE,         "Fake",         0x1B, '\xB2', 0xFF, -1)
XX(INVISIBLE,    "Invisible",    0x1C, '\xB0', 0xFF, -1)

XX(BLINK_WALL,   "Blink wall",   0x1D, '\xCE', 0xFF,  1)
XX(TRANSPORTER,  "Transporter",  0x1E, '\xC5', 0xFF,  2)
XX(LINE,         "Line",         0x1F, '\xCE', 0xFF, -1)
XX(RICOCHET,     "Ricochet",     0x20, '\x2A', 0x0A, -1)
#if !SUPER_ZZT
XX(BLINK_RAY_H,  "",             0x21, '\xCD', 0xFF, -1)
#endif /* !SUPER_ZZT */
#if SUPER_ZZT
XX(BEAR,         "Bear",         0x22, '\xEB', 0x02,  3)
#else /* !SUPER_ZZT */
XX(BEAR,         "Bear",         0x22, '\x99', 0x06,  3)
#endif /* SUPER_ZZT */
XX(RUFFIAN,      "Ruffian",      0x23, '\x05', 0x0D,  1)
XX(OBJECT,       "Object",       0x24, '\x02', 0xFF,  3)
XX(SLIME,        "Slime",        0x25, '\x2A', 0xFF,  3)
#if !SUPER_ZZT
XX(SHARK,        "Shark",        0x26, '\x5E', 0x07,  3)
#endif /* !SUPER_ZZT */
XX(SPINNING_GUN, "Spinning gun", 0x27, '\x18', 0xFF,  2)
XX(PUSHER,       "Pusher",       0x28, '\x10', 0xFF,  4)
XX(LION,         "Lion",         0x29, '\xEA', 0x0C,  2)
XX(TIGER,        "Tiger",        0x2A, '\xE3', 0x0B,  2)
#if !SUPER_ZZT
XX(BLINK_RAY_V,  "",             0x2B, '\xBA', 0xFF, -1)
#endif /* !SUPER_ZZT */
XX(HEAD,         "Head",         0x2C, '\xE9', 0xFF,  2)
XX(SEGMENT,      "Segment",      0x2D, '\x4F', 0xFF,  2)
// 0x2E blank

#if SUPER_ZZT
XX(FLOOR,        "Floor",        0x2F, '\xB0', 0xFF, -1)
XX(WATER_N,      "Water N",      0x30, '\x1E', 0x19, -1)
XX(WATER_S,      "Water S",      0x31, '\x1F', 0x19, -1)
XX(WATER_W,      "Water W",      0x32, '\x11', 0x19, -1)
XX(WATER_E,      "Water E",      0x33, '\x10', 0x19, -1)
// 0x34 through 0x3A blank

XX(ROTON,        "Roton",        0x3B, '\x94', 0x0D,  1)
XX(DRAGON_PUP,   "Dragon pup",   0x3C, '\xED', 0x04,  2)
XX(PAIRER,       "Pairer",       0x3D, '\xE5', 0x01,  2)
XX(SPIDER,       "Spider",       0x3E, '\x0F', 0xFF,  1)
XX(WEB,          "Web",          0x3F, '\xC5', 0xFF, -1)
XX(STONE,        "Stone",        0x40, '\x5A', 0x0F,  1)
// 0x41 through 0x44 blank

XX(BULLET,       "Bullet",       0x45, '\xF8', 0x0F,  1)
XX(BLINK_RAY_H,  "",             0x46, '\xCD', 0xFF, -1)
XX(BLINK_RAY_V,  "",             0x47, '\xBA', 0xFF, -1)
XX(STAR,         "Star",         0x48, '\x53', 0x0F,  1)
XX(TEXT_BLUE,    "",             0x49, '\x20', 0xFF, -1)
XX(TEXT_GREEN,   "",             0x4A, '\x20', 0xFF, -1)
XX(TEXT_CYAN,    "",             0x4B, '\x20', 0xFF, -1)
XX(TEXT_RED,     "",             0x4C, '\x20', 0xFF, -1)
XX(TEXT_PURPLE,  "",             0x4D, '\x20', 0xFF, -1)
XX(TEXT_BROWN,   "",             0x4E, '\x20', 0xFF, -1)
XX(TEXT_BLACK,   "",             0x4F, '\x20', 0xFF, -1)
#else /* !SUPER_ZZT */
XX(TEXT_BLUE,    "",             0x2F, '\x20', 0xFF, -1)
XX(TEXT_GREEN,   "",             0x30, '\x20', 0xFF, -1)
XX(TEXT_CYAN,    "",             0x31, '\x20', 0xFF, -1)
XX(TEXT_RED,     "",             0x32, '\x20', 0xFF, -1)
XX(TEXT_PURPLE,  "",             0x33, '\x20', 0xFF, -1)
XX(TEXT_BROWN,   "",             0x34, '\x20', 0xFF, -1)
XX(TEXT_BLACK,   "",             0x35, '\x20', 0xFF, -1)
#endif /* SUPER_ZZT */

