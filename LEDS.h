#include <Adafruit_NeoPixel.h>
#define PIN        8
#define NUMPIXELS 6 // Popular NeoPixel ring size
int BRIGHTNESS = 64;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);

//////////////////////////////
// LEDs
//////////////////////////////
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

int default_leds [3][6] = {{13, 1, 2, 11, 14, 15}, {22, 24, 26, 25, 18, 19}, {1, 6, 7, 42, 9, 8}};
class Led {
  private:
    byte num;
    bool valueinarray(int val, int *arr) {
      for (byte i = 0; i < sizeof(arr); i++) {
         if (arr[i] == val) return true;
         break;
       }
       return false;
     }

    void show_white() {
      pixels.setPixelColor(num, pixels.Color(100, 100, 100));
      pixels.show();
    }

  public:
    Led(byte number) {
      num = number;
      pixels.begin();
      for(int i=0 ; i<NUM_LAYOUT ; i++){
        led_control[i] = default_leds[i][num];
      }
    };
    byte r = 0;
    byte g = 0;
    byte b = 0;
    bool toggle_fast = HIGH;
    bool toggle_slow = HIGH;
    int led_type[NUM_LAYOUT] = {0, 0, 0};
    int led_control[NUM_LAYOUT] = {0, 0, 0};
    int led_channel[NUM_LAYOUT] = {16, 16, 16};
    void set_color(byte color, byte _state) {
      r = color_index[color][0];
      g = color_index[color][1];
      b = color_index[color][2];
      led_channel[current_layout] = _state;
      if (color == 0) {
          r = 0;
          g = 0;
          b = 0;
        int  controls_list[9] = {13, 14, 15, 18, 19, 22, 28, 35, 41}; // list of controls that rely on color_index
        if (valueinarray(led_control[current_layout], controls_list)) { // if value = 0 and the control corresponds to a color_index then get color_index[0]
          r = color_index[color][0];
          g = color_index[color][1];
          b = color_index[color][2];
        }
      }
    }

    void show_color() {
      pixels.setBrightness(BRIGHTNESS);
      pixels.setPixelColor(num, r * 2, g * 2, b * 2);
      pixels.show();
    }

    void led_update(bool button_state) {
     if (!button_state) {
        show_color();
     }
     else show_white();
    }

    void toggle_led(byte beat) {
      if (led_channel[current_layout] == 14) {
        if (beat == 2 || beat == 4) show_color();
        else led_off();
      }
      if (led_channel[current_layout] == 15) {
        if (beat == 1 || beat == 4) show_color();
        else led_off();
      }
    }

    void led_off() {
      pixels.setPixelColor(num, pixels.Color(0, 0, 0));
      pixels.show();;
    }
};

Led l[NUM_LEDS] = {Led(0), Led(1), Led(2), Led(3), Led(4), Led(5)};

void init_LEDS() {
  byte init_led_color_red[NUM_LEDS] = {80, 0, 17, 100, 124, 90};
  byte init_led_color_green[NUM_LEDS] = {0, 78, 23, 100, 49, 0};
  byte init_led_color_blue[NUM_LEDS] = {100, 46, 80, 0, 0, 12};
  for ( byte i = 0; i < NUM_LEDS; i ++) {
    l[i].r = init_led_color_red[i];
    l[i].g = init_led_color_green[i];
    l[i].b = init_led_color_blue[i];
    l[i].show_color();
    delay(1);
  }
}
