#include <Adafruit_NeoPixel.h>
#define PIN        8
#define NUMPIXELS 6 // Popular NeoPixel ring size
int BRIGHTNESS = 64;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);
int fade_resolution = 24 ;

//////////////////////////////
// LEDs
//////////////////////////////

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
    void set_color(byte color, byte channel) {
      r = color_index[color][0];
      g = color_index[color][1];
      b = color_index[color][2];
      led_channel[current_layout] = channel;
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
      pixels.setPixelColor(num, r * 2, g * 2, b * 2);
      pixels.show();
    }
    
    void show_direct_color(int r, int g, int b) {
      pixels.setPixelColor(num, r * 2, g * 2, b * 2);
      pixels.show();
    }

    void show_color_fade(int fade_amount) {
      USB_MIDI.sendControlChange(2, fade_amount, 2);
      pixels.setPixelColor(num, int(r * 2 * fade_amount/fade_resolution), int(g * 2 * fade_amount/fade_resolution), int(b * 2 * fade_amount/fade_resolution));
      pixels.show();
    }

    void led_update(bool button_state) {
     if (!button_state) {
        show_color();
     }
     else show_white();
    }

    void fade_slow(int beat) {
     // if (beat == 1 || beat == 2) led_off();
       show_color_fade(max(fade_resolution-beat, 0));
    //  delay(1);
    } 

    void blink_slow(int beat) {
      if (beat == 2 || beat == 3) led_off();
      else  show_color();
      delay(1);
    } 

     void blink_fast(byte beat) {
      if (beat == 1 || beat == 3 ) led_off();
      else show_color();
      delay(1);
    } 

    void show_green() {
      pixels.setPixelColor(num, 0, 255, 0);
      pixels.show();
    }

   void led_off() {
      pixels.setPixelColor(num, 0, 0, 0);
      pixels.show();
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

void clear_leds(bool midiUSB) {
  for (byte i = 0; i < NUM_LEDS; ++i) {
    l[i].r = 0;
    l[i].g = 0;
    l[i].b = 0;
    l[i].show_color();
    if (midiUSB) delay(1);
  }
}
