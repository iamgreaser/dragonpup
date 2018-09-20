XX(EMPTY,        "Empty",        0x00)
XX(BOARD_EDGE,   "",             0x01)
XX(MESSENGER,    "Messenger",    0x02)
XX(MONITOR,      "Monitor",      0x03)
XX(PLAYER,       "Player",       0x04)
XX(AMMO,         "Ammo",         0x05)
#if !SUPER_ZZT
XX(TORCH,        "Torch",        0x06)
#endif /* !SUPER_ZZT */
XX(GEM,          "Gem",          0x07)
XX(KEY,          "Key",          0x08)
XX(DOOR,         "Door",         0x09)
XX(SCROLL,       "Scroll",       0x0A)
XX(PASSAGE,      "Passage",      0x0B)
XX(DUPLICATOR,   "Duplicator",   0x0C)
XX(BOMB,         "Bomb",         0x0D)
XX(ENERGIZER,    "Energizer",    0x0E)
#if !SUPER_ZZT
XX(STAR,         "Star",         0x0F)
#endif /* !SUPER_ZZT */
XX(CLOCKWISE,    "Clockwise",    0x10)
XX(COUNTER,      "Counter",      0x11)
#if !SUPER_ZZT
XX(BULLET,       "Bullet",       0x12)
#endif /* !SUPER_ZZT */
#if SUPER_ZZT
XX(LAVA,         "Lava",         0x13)
#else /* !SUPER_ZZT */
XX(WATER,        "Water",        0x13)
#endif /* SUPER_ZZT */
XX(FOREST,       "Forest",       0x14)
XX(SOLID,        "Solid",        0x15)
XX(NORMAL,       "Normal",       0x16)
XX(BREAKABLE,    "Breakable",    0x17)
XX(BOULDER,      "Boulder",      0x18)
XX(SLIDER_NS,    "Slider (NS)",  0x19)
XX(SLIDER_EW,    "Slider (EW)",  0x1A)
XX(FAKE,         "Fake",         0x1B)
XX(INVISIBLE,    "Invisible",    0x1C)

XX(BLINK_WALL,   "Blink wall",   0x1D)
XX(TRANSPORTER,  "Transporter",  0x1E)
XX(LINE,         "Line",         0x1F)
XX(RICOCHET,     "Ricochet",     0x20)
#if !SUPER_ZZT
XX(BLINK_RAY_H,  "",             0x21)
#endif /* !SUPER_ZZT */
XX(BEAR,         "Bear",         0x22)
XX(RUFFIAN,      "Ruffian",      0x23)
XX(OBJECT,       "Object",       0x24)
XX(SLIME,        "Slime",        0x25)
#if !SUPER_ZZT
XX(SHARK,        "Shark",        0x26)
#endif /* !SUPER_ZZT */
XX(SPINNING_GUN, "Spinning gun", 0x27)
XX(PUSHER,       "Pusher",       0x28)
XX(LION,         "Lion",         0x29)
XX(TIGER,        "Tiger",        0x2A)
#if !SUPER_ZZT
XX(BLINK_RAY_V,  "",             0x2B)
#endif /* !SUPER_ZZT */
XX(HEAD,         "Head",         0x2C)
XX(SEGMENT,      "Segment",      0x2D)
// 0x2E blank

#if SUPER_ZZT
XX(FLOOR,        "Floor",        0x2F)
XX(WATER_N,      "Water N",      0x30)
XX(WATER_S,      "Water S",      0x31)
XX(WATER_W,      "Water W",      0x32)
XX(WATER_E,      "Water E",      0x33)
// 0x34 through 0x3a blank

XX(ROTON,        "Roton",        0x3B)
XX(DRAGON_PUP,   "Dragon pup",   0x3C)
XX(PAIRER,       "Pairer",       0x3D)
XX(SPIDER,       "Spider",       0x3E)
XX(WEB,          "Web",          0x3F)
XX(STONE,        "Stone",        0x40)
// 0x41 through 0x44 blank

XX(BULLET,       "Bullet",       0x45)
XX(BLINK_RAY_H,  "",             0x46)
XX(BLINK_RAY_V,  "",             0x47)
XX(STAR,         "Star",         0x48)
XX(TEXT_BLUE,    "",             0x49)
XX(TEXT_GREEN,   "",             0x4A)
XX(TEXT_CYAN,    "",             0x4B)
XX(TEXT_RED,     "",             0x4C)
XX(TEXT_PURPLE,  "",             0x4D)
XX(TEXT_BROWN,   "",             0x4E)
XX(TEXT_BLACK,   "",             0x4F)
#else /* !SUPER_ZZT */
XX(TEXT_BLUE,    "",             0x2F)
XX(TEXT_GREEN,   "",             0x30)
XX(TEXT_CYAN,    "",             0x31)
XX(TEXT_RED,     "",             0x32)
XX(TEXT_PURPLE,  "",             0x33)
XX(TEXT_BROWN,   "",             0x34)
XX(TEXT_BLACK,   "",             0x35)
#endif /* SUPER_ZZT */

