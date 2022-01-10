#define  latch_Col 24
#define  clock_Col 23
#define  data_Col 25
#define  toggle_Col 22
byte const  row_pins[6] = { 16, 17, 18, 19, 20, 21 };

// LED Matrix
#define MAX_CONCAT 99
#define  MAX_CHAR 20
int column_data[34] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int _tick = 0;
int matrix_brightness = 0;

class Display {
private:
  int index;
  int scroller;
  int scroller_temp;
  int ticker;

  void clear_concat() {
    for (int i = 0; i < MAX_CONCAT; i++) {
      concat_text[i] = 0;
    }
  }

public:
  int layout[NUM_LAYOUT] = { 0, 1, 4};
  int text_len = 5;
  // int text[MAX_CHAR] = {79, 80, 69, 78, 14, 67, 79, 78, 84, 82, 79, 76};
  // int text[MAX_CHAR] = {96, 97,98, 99, 100, 101, 102, 103, 104};
  int temp_text[MAX_CHAR];
  int data_text[MAX_CHAR] = { 79, 80, 105, 67, 79 };
  int page_text[MAX_CHAR] = { 48, 65, 71, 69, 0, 79 };

  int concat_text[MAX_CONCAT];

  void clear_text() {
    for (int i = 0; i < MAX_CHAR; i++) {
      data_text[i] = 0;
    }
  }
  
  void build_text(int _text_len, int *text) {
    index = 0;
    scroller_temp = 0;
    scroller = 0;
    scrolled = LOW;
    clear_concat();
  //  text_len = text_len;
    if (_text_len < 8) index = (36 - (_text_len * 4)) / 2;
    for (int i = 0; i < _text_len; i++) {
      for (int j = 0; j < 3; j++) {
        if (index < MAX_CONCAT) concat_text[index] = font_5x7[text[i]][j];
        if (font_5x7[text[i]][j] != 0) index++;
      }
      index++;
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


  void _set(int s, bool v) {
    digitalWrite(s, v);
  }

  void _set_analog(int s, int v) {
    analogWrite(s, v);
  }

  void tick(int s) {
    _set(s, LOW);
      _set(s, HIGH);
  }

  void show_char(int i) {
    int temp = concat_text[i - 1 + scroller];
    int data_col = column_data[i];
    _set(data_Col, data_col);
    _set(toggle_Col, HIGH);
  // _set_analog(toggle_Col, 1050);
    for (int j = 0; j < 8; j++) {
      bool data_row = !(temp & (0x80));
      if (7 - j < 6) _set(row_pins[7 - j], data_row);
      temp <<= 1;
    }
    tick(latch_Col);
    _set_analog(toggle_Col, matrix_brightness);
    tick(clock_Col);
  };
};

Display disp;


void Display_Handler()
{ 
  _tick ++;
  if (_tick == 34) _tick = 0;
  disp.show_char(_tick);
}
