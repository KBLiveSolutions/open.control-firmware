// LED Matrix
byte const MAX_CONCAT = 99;
byte font_5x7[106][3] = {
  {0x00, 0x00, 0x00},  //
  {0x00, 0x3a, 0x00},  // !
  {0x30, 0x00, 0x30},  // "
  {0x3E, 0x14, 0x3E},  // #
  {0x14, 0x3E, 0x28},  // $
  {0x24, 0x08, 0x12},  // %
  {0x3C, 0x3A, 0x0E},  // &
  {0x00, 0x70, 0x00},  // '
  {0x00, 0x1C, 0x22 },  // (
  {0x22, 0x1C, 0x00},  // )
  {0x28, 0x10, 0x28},  // *
  {0x08, 0x1C, 0x08},  // +
  {0x02, 0x04, 0x00},  //
  {0x08, 0x08, 0x08},  // -
  {0x00, 0x02, 0x00},  // .
  {0x04, 0x08, 0x10},  // /
  {0x1C, 0x2A, 0x1C}, // 0
  {0x12, 0x3e, 0x02}, // 1
  {0x26, 0x2a, 0x12}, // 2
  {0x22, 0x2A, 0x14}, // 3
  {0x38, 0x08, 0x1e}, // 4
  {0x3a, 0x2a, 0x24}, // 5
  {0x1C, 0x2a, 0x2c}, // 6
  {0x26, 0x28, 0x30}, // 7
  {0x14, 0x2A, 0x14}, // 8
  {0x1A, 0x2A, 0x1C}, // 9
  {0x00, 0x14, 0x00},  // :
  {0x02, 0x14, 0x00},  // ;
  {0x08, 0x14, 0x22},  // <
  {0x14, 0x14, 0x14},  // =
  {0x22, 0x14, 0x08},  // >
  {0x20, 0x2A, 0x30},  // ?
  {0x1C, 0x2A, 0x1A},  // @
  {0x1E, 0x28, 0x1E},  // A
  {0x3E, 0x2A, 0x14},  // B
  {0x1C, 0x22, 0x22},  // C
  {0x3E, 0x22, 0x1C},  // D
  {0x3E, 0x2A, 0x22},  // E
  {0x3E, 0x28, 0x20},  // F
  {0x1C, 0x2A, 0x2E},  // G
  {0x3E, 0x08, 0x3E},  // H
  {0x22, 0x3E, 0x22},  // I
  {0x04, 0x02, 0x3C},  // J
  {0x3E, 0x08, 0x36},  // K
  {0x3E, 0x02, 0x02},  // L
  {0x3E, 0x18, 0x3E},  // M
  {0x3E, 0x1C, 0x3E},  // N
  {0x1C, 0x22, 0x1C},  // O
  {0x3E, 0x28, 0x10},  // P
  {0x1c, 0x26, 0x1E},  // Q
  {0x3E, 0x2c, 0x1A},  // R
  {0x12, 0x2A, 0x24},  // S
  {0x20, 0x3E, 0x20},  // T
  {0x3C, 0x02, 0x3E},  // U
  {0x3C, 0x02, 0x3C},  // V
  {0x3E, 0x0C, 0x3E},  // W
  {0x36, 0x08, 0x36},  // X
  {0x70, 0x0E, 0x70},  // Y
  {0x26, 0x2A, 0x32},  // Z
  {0x3E, 0x22, 0x22},  // [
  {0x10, 0x08, 0x04},  // "
  {0x22, 0x22, 0x3E},  // ]
  {0x10, 0x20, 0x10},  // ^
  {0x02, 0x02, 0x02},  // _
  {0x20, 0x10, 0x00},  // `
  {0x16, 0x1A, 0x0E},  // a
  {0x3E, 0x12, 0x0C},  // b
  {0x0C, 0x12, 0x12},  // c
  {0x0C, 0x12, 0x3E},  // d
  {0x0C, 0x16, 0x1A},  // e
  {0x08, 0x1E, 0x28},  // f
  {0x0C, 0x15, 0x1E},  // g
  {0x3E, 0x10, 0x0E},  // h
  {0x00, 0x2E, 0x00},  // i   {0x0A, 0x2E, 0x02},
  {0x02, 0x01, 0x2E},  // j
  {0x3E, 0x0C, 0x12},  // k
  {0x22, 0x3E, 0x02},  // l
  {0x1E, 0x1C, 0x1E},  // m
  {0x1E, 0x10, 0x0E},  // n
  {0x0C, 0x12, 0x0C},  // o
  {0x1f, 0x12, 0x0C},  // p
  {0x0C, 0x12, 0x1F},  // q
  {0x0E, 0x10, 0x10},  // r
  {0x0A, 0x1E, 0x14},  // s
  {0x10, 0x3E, 0x12},  // t
  {0x1C, 0x02, 0x1E},  // u
  {0x1C, 0x06, 0x1C},  // v
  {0x1e, 0x06, 0x1e}, // w
  {0x12, 0x0C, 0x12},  // x
  {0x18, 0x05, 0x1E},  // y
  {0x16, 0x1E, 0x1A},  // z
  {0x08, 0x36, 0x22},  // {
  {0x00, 0x36, 0x00},  // |
  {0x22, 0x36, 0x08},  // }
  {0x08, 0x18, 0x10},  // ~
  {0x1C, 0x14, 0x1C},  // []
  {0x10, 0x28, 0x10},  // open.control caracter 1
  {0x3C, 0x28, 0x10},  // open.control caracter 2
  {0x3A, 0x3D, 0x2D},  // open.control caracter 3
  {0x3A, 0x25, 0x1A},  // open.control caracter 4
  {0x17, 0x04, 0x03},  // open.control caracter 5
  {0x0E, 0x05, 0x00},  // open.control caracter 6
  {0x07, 0x04, 0x00},  // open.control caracter 7
  {0x02, 0x05, 0x02},  // open.control caracter 8
  {0x0F, 0x00, 0x00},  // open.control caracter 9
  {0x04, 0x00, 0x00}  // open.control caracter 9
  
};


byte column_data[32] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

class Display {
  private:
    byte text_len;
    byte index;
    byte scroller;
    byte  scroller_temp;
    byte ticker;

    void clear_concat() {
      for (byte i = 0; i < MAX_CONCAT; i++) {
        concat_text[i] = 0;
      }
    }

  public:
    byte layout[NUM_LAYOUT] = {0, 0, 0};
   // byte text[MAX_CHAR] = {79, 80, 69, 78, 14, 67, 79, 78, 84, 82, 79, 76};
   // byte text[MAX_CHAR] = {96, 97,98, 99, 100, 101, 102, 103, 104};
    byte text[MAX_CHAR] = {79, 80,  105, 67, 79};
    
    byte concat_text[MAX_CONCAT];

    void clear_text() {
      for (byte i = 0; i < MAX_CHAR; i++) {
        text[i] = 0;
      }
    }
    void build_text(byte text_len) {
      index = 0;
      scroller_temp = 0;
      scroller = 0;
      scrolled = LOW;
      clear_concat();
      text_len = text_len;
      if (text_len < 8) index = (36 - (text_len * 4)) / 2;
      for (int i = 0; i < text_len ; i++) {
        for (byte j = 0; j < 3; j++) {
          if (index < MAX_CONCAT) concat_text[index] = font_5x7[text[i]][j];
          if (font_5x7[text[i]][j] != 0) index ++;
        }
        index ++;
      }
    }
    bool scrolled = LOW;
    void inc_scroll() {
      scroller_temp++;
      if (scroller_temp > 5 && index > MAX_CHAR && !scrolled) scroller++;
      if (scroller > (index - 32)) {
        scroller_temp = 0;
        scroller = 0;
        scrolled = HIGH;
      }
    }
    void tick_on() {
    //  PORTC &= ~(1 << PORTC7);  //C7 toggle_Col
    //  PORTD &= ~(1 << PORTD7);  //D7 toggle_Row
   //   delayMicroseconds(94);
    }


    void tick_off() {
   //  analogWrite(toggle_Col, 200);
      PORTF |= 1 << PORTF1; //F1 latch_Col
      PORTF &= ~(1 << PORTF1);  //F1 latch_Col
      PORTD |= 1 << PORTD6; //D6 latch_Row
      PORTD &= ~(1 << PORTD6);  //D6 latch_Row
   //  analogWrite(toggle_Col, 0);
  //    PORTD |= 1 << PORTD7;;  //D7 toggle_Row
  //   PORTC |= 1 << PORTC7;;  //C7 toggle_Col
     analogWrite(toggle_Col, matrix_brightness);
    }

    void show_char(int i) {
    //  tick_on();
    //   delayMicroseconds(14);
      int temp = concat_text[i - 1 + scroller];
      byte data_column = column_data[i];
        if (data_column) PORTF |= 1 << PORTF0;  // F0 data_Col
        else PORTF&= ~(1 << PORTF0);  // F0 data_Col
      for (int j = 0; j < 8; j++) {
        byte data_row = (temp & (0x80));
        if (data_row) PORTB |= 1 << PORTB4;   // B4 data_Row
        else PORTB &= ~(1 << PORTB4);   // B4 data_Row
        temp <<= 1;
        PORTD &= ~(1 << PORTD4);  //D4 clock_Row
        PORTD |= 1 << PORTD4;   //D4 clock_Row
      }
      PORTF &= ~(1 << PORTF4);  //F4 clock_Col
      PORTF |= 1 << PORTF4;  //F4 clock_Col
      tick_off();
    }
};

Display disp;
