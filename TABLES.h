byte current_layout = 0;
byte options[NUM_OPTIONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte linked_page[NUM_LAYOUT] = {0, 0, 0};


// Factory default button values for Pages 1 to 3. Overriden as soon as they are changed with the Web Editor
byte default_short[3][NUM_BUTTONS] = {{13, 1, 2, 11, 14, 15, 56, 57}, {22, 24, 26, 11, 18, 19, 56, 57}, {103, 1, 7, 6, 9, 8, 56, 57}};
byte default_long[3][NUM_BUTTONS] = {{17, 104, 41, 43, 4, 40, 56, 57}, {0, 55, 20, 21, 4, 40, 56, 57}, {42, 41, 102, 10, 4, 40, 56, 57}};
byte default_double[3][NUM_BUTTONS] = {{16, 75, 76, 5, 105, 106, 56, 57}, {23, 27, 11, 5, 18, 19, 56, 57}, {74, 75, 76, 5, 38, 39, 56, 57}};

int default_leds [3][6] = {{13, 1, 2, 11, 14, 15}, {22, 24, 26, 25, 18, 19}, {42, 1, 7, 6, 38, 39}};

byte default_rotary [3][2] = {{89, 73}, {91, 73}, {99, 83}};
byte default_rotary_hold [3][2] = {{90, 87}, {96, 101}, {101, 84}};

byte default_sliders [3][2] = {{0, 0}, {0, 0}, {0, 0}};
// byte default_sliders [3][2] = {{89, 90}, {91, 96}, {73, 37}};


byte external_MIDI_control[NUM_BUTTONS + NUM_SLIDERS] = {1, 2, 3, 4, 5, 6, 7, 8};
byte external_MIDI_channel[NUM_BUTTONS + NUM_SLIDERS] = {1, 1, 1, 1, 1, 1, 1, 1};
byte external_MIDI_type[NUM_BUTTONS + NUM_SLIDERS] = {0, 0, 0, 0, 0, 0, 0, 0};


        
int font_5x7[106][3] = {
  { 0x00, 0x00, 0x00 },  //
  { 0x00, 0x3a, 0x00 },  // !
  { 0x30, 0x00, 0x30 },  // "
  { 0x3E, 0x14, 0x3E },  // #
  { 0x14, 0x3E, 0x28 },  // $
  { 0x24, 0x08, 0x12 },  // %
  { 0x3C, 0x3A, 0x0E },  // &
  { 0x00, 0x70, 0x00 },  // '
  { 0x00, 0x1C, 0x22 },  // (
  { 0x22, 0x1C, 0x00 },  // )
  { 0x28, 0x10, 0x28 },  // *
  { 0x08, 0x1C, 0x08 },  // +
  { 0x02, 0x04, 0x00 },  //
  { 0x08, 0x08, 0x08 },  // -
  { 0x00, 0x02, 0x00 },  // .
  { 0x04, 0x08, 0x10 },  // /
  { 0x1C, 0x2A, 0x1C },  // 0
  { 0x12, 0x3e, 0x02 },  // 1
  { 0x26, 0x2a, 0x12 },  // 2
  { 0x22, 0x2A, 0x14 },  // 3
  { 0x38, 0x08, 0x1e },  // 4
  { 0x3a, 0x2a, 0x24 },  // 5
  { 0x1C, 0x2a, 0x2C },  // 6
  { 0x26, 0x28, 0x30 },  // 7
  { 0x14, 0x2A, 0x14 },  // 8
  { 0x1A, 0x2A, 0x1C },  // 9
  { 0x00, 0x14, 0x00 },  // :
  { 0x02, 0x14, 0x00 },  // ;
  { 0x08, 0x14, 0x22 },  // <
  { 0x14, 0x14, 0x14 },  // =
  { 0x22, 0x14, 0x08 },  // >
  { 0x20, 0x2A, 0x30 },  // ?
  { 0x1C, 0x2A, 0x1A },  // @
  { 0x1E, 0x28, 0x1E },  // A
  { 0x3E, 0x2A, 0x14 },  // B
  { 0x1C, 0x22, 0x22 },  // C
  { 0x3E, 0x22, 0x1C },  // D
  { 0x3E, 0x2A, 0x22 },  // E
  { 0x3E, 0x28, 0x20 },  // F
  { 0x1C, 0x2A, 0x2E },  // G
  { 0x3E, 0x08, 0x3E },  // H
  { 0x22, 0x3E, 0x22 },  // I
  { 0x04, 0x02, 0x3C },  // J
  { 0x3E, 0x08, 0x36 },  // K
  { 0x3E, 0x02, 0x02 },  // L
  { 0x3E, 0x10, 0x3E },  // M
  { 0x3E, 0x18, 0x3E },  // N { 0x3E, 0x10, 0x3E }
  { 0x1C, 0x22, 0x1C },  // O
  { 0x3E, 0x28, 0x10 },  // P
  { 0x1c, 0x26, 0x1E },  // Q
  { 0x3E, 0x2c, 0x1A },  // R
  { 0x12, 0x2A, 0x24 },  // S
  { 0x20, 0x3E, 0x20 },  // T
  { 0x3C, 0x02, 0x3E },  // U
  { 0x3C, 0x02, 0x3C },  // V
  { 0x3E, 0x0C, 0x3E },  // W
  { 0x36, 0x08, 0x36 },  // X
  { 0x70, 0x0E, 0x70 },  // Y
  { 0x26, 0x2A, 0x32 },  // Z
  { 0x3E, 0x22, 0x00 },  // [
  { 0x10, 0x08, 0x04 },  // "
  { 0x00, 0x22, 0x3E },  // ]
  { 0x10, 0x20, 0x10 },  // ^
  { 0x02, 0x02, 0x02 },  // _
  { 0x20, 0x10, 0x00 },  // `
  { 0x16, 0x1A, 0x0E },  // a
  { 0x3E, 0x12, 0x0C },  // b
  { 0x0C, 0x12, 0x12 },  // c
  { 0x0C, 0x12, 0x3E },  // d
  { 0x0C, 0x16, 0x1A },  // e
  { 0x08, 0x1E, 0x28 },  // f
  { 0x0C, 0x15, 0x1E },  // g
  { 0x3E, 0x10, 0x0E },  // h
  { 0x00, 0x2E, 0x00 },  // i   {0x0A, 0x2E, 0x02},
  { 0x02, 0x01, 0x2E },  // j
  { 0x3E, 0x0C, 0x12 },  // k
  { 0x22, 0x3E, 0x02 },  // l
  { 0x1E, 0x1C, 0x1E },  // m
  { 0x1E, 0x10, 0x0E },  // n
  { 0x0C, 0x12, 0x0C },  // o
  { 0x1f, 0x12, 0x0C },  // p
  { 0x0C, 0x12, 0x1F },  // q
  { 0x0E, 0x10, 0x10 },  // r
  { 0x0A, 0x1E, 0x14 },  // s
  { 0x10, 0x3E, 0x12 },  // t
  { 0x1C, 0x02, 0x1E },  // u
  { 0x1C, 0x06, 0x1C },  // v
  { 0x1e, 0x06, 0x1e },  // w
  { 0x12, 0x0C, 0x12 },  // x
  { 0x18, 0x05, 0x1E },  // y
  { 0x16, 0x1E, 0x1A },  // z
  { 0x08, 0x36, 0x22 },  // {
  { 0x00, 0x36, 0x00 },  // |
  { 0x22, 0x36, 0x08 },  // }
  { 0x08, 0x18, 0x10 },  // ~
  { 0x1C, 0x14, 0x1C },  // []
  { 0x10, 0x28, 0x10 },  // open.control caracter 1
  { 0x3C, 0x28, 0x10 },  // open.control caracter 2
  { 0x3A, 0x3D, 0x2D },  // open.control caracter 3
  { 0x3A, 0x25, 0x1A },  // open.control caracter 4
  { 0x17, 0x04, 0x03 },  // open.control caracter 5
  { 0x0E, 0x05, 0x00 },  // open.control caracter 6
  { 0x07, 0x04, 0x00 },  // open.control caracter 7
  { 0x02, 0x05, 0x02 },  // open.control caracter 8
  { 0x0F, 0x00, 0x00 },  // open.control caracter 9
  { 0x04, 0x00, 0x00 }   // open.control caracter 9
};


byte const color_index[128][3] = {
  {100, 25, 32}, //0
  {114, 39, 0},
  {85, 44, 8},
  {76, 49, 9},
  {52, 62, 1},
  {55, 88, 0},
  {55, 77, 16},
  {30, 61, 29},
  {19, 21, 50},
  {14, 3, 66},
  {43, 24, 66}, //10
  {107, 6, 59},
  {127, 6, 28},
  {83, 59, 30},
  {114, 10, 1},
  {127, 28, 0},
  {59, 22, 1},
  {81, 56, 0},
  {46, 60, 2},
  {51, 98, 5},
  {40, 86, 36}, //20
  {40, 76, 64},
  {29, 52, 99},
  {12, 12, 99},
  {75, 0, 99},
  {112, 1, 95},
  {112, 1, 29},
  {92, 70, 30},
  {100, 32, 12},
  {106, 39, 0},
  {74, 39, 4},  //30
  {72, 67, 11},
  {74, 80, 11},
  {76, 100, 0},
  {63, 100, 18},
  {59, 77, 35},
  {58, 86, 66},
  {72, 53, 66},
  {72, 38, 66},
  {85, 11, 81},
  {97, 64, 62}, //40
  {90, 64, 48},
  {114, 42, 32},
  {127, 53, 0},
  {101, 48, 0},
  {49, 49, 0},
  {47, 69, 0},
  {51, 76, 7},
  {51, 90, 44},
  {46, 53, 44},
  {42, 54, 67},  //50
  {53, 36, 88},
  {74, 26, 87},
  {87, 32, 49},
  {93, 17, 25},
  {47, 40, 25},
  {51, 0, 0},
  {51, 9, 0},
  {16, 6, 0},
  {47, 38, 0},
  {60, 79, 0},  //60
  {40, 79, 0},
  {40, 94, 33},
  {0, 25, 43},
  {0, 0, 54},
  {0, 55, 0},
  {42, 0, 52},
  {64, 0, 52},
  {83, 0, 35},
  {12, 10, 8},
  {93, 23, 0}, //70
  {32, 13, 0},
  {71, 12, 0},
  {28, 8, 0},
  {59, 43, 20},
  {28, 19, 10},
  {37, 14, 5},
  {13, 6, 2},
  {100, 88, 23},
  {32, 28, 7},
  {102, 78, 8}, // 80
  {33, 25, 2},
  {72, 102, 5},
  {23, 33, 1},
  {48, 102, 9},
  {15, 33, 3},
  {20, 77, 8},
  {6, 25, 2},
  {31, 55, 1},
  {10, 17, 0},
  {39, 102, 34}, // 90
  {12, 33, 11},
  {20, 62, 41},
  {8, 25, 16},
  {0, 24, 14},
  {19, 69, 102},
  {6, 22, 33},
  {21, 39, 100},
  {7, 12, 32},
  {10, 20, 102},
  {3, 6, 33}, // 100
  {11, 4, 92},
  {3, 1, 29},
  {10, 28, 76},
  {4, 11, 30},
  {22, 22, 102},
  {34, 13, 102},
  {15, 0, 0},
  {60, 17, 102},
  {19, 5, 33},
  {53, 13, 48}, // 110
  {17, 4, 15},
  {102, 6, 20},
  {33, 2, 6},
  {102, 17, 84},
  {33, 5, 27},
  { 0, 0, 0},
  {0, 0, 0},
  {89, 89, 89},
  {26, 26, 26},
  {255, 255, 255}, // 120
  {89, 89, 89},
  {153, 153, 153},
  {64, 64, 64},
  {20, 20, 20},
  {0, 0, 255},
  {0, 255, 0},
  {255, 0, 0}
};
