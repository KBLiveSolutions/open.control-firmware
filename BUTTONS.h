//////////////////////////////
// BUTTONS
//////////////////////////////

int hold_button_time = 800;
const int b_pins[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, 14, 13};

byte default_short[3][NUM_BUTTONS] = {{13, 1, 2, 11, 14, 15, 56, 57}, {22, 24, 26, 25, 18, 19, 56, 57}, {1, 6, 7, 75, 9, 8, 56, 57}};
byte default_long [3][NUM_BUTTONS] = {{16, 17, 3, 43, 4, 40, 56, 57}, {23, 27, 55, 20, 21, 5, 56, 57}, {100, 103, 4, 42, 10, 41, 56, 57}};

unsigned long debounceDelay = 10;

class Button {
  private:
    unsigned long time_pressed = 0;
  public:
    byte num;
    Button(byte number) {
      num = number;
      pinMode(b_pins[num], INPUT_PULLUP);
      for (int i = 0 ; i < NUM_LAYOUT ; i++) {
        short_control[i] = default_short[i][num];
        long_control[i] = default_long[i][num];
      }
    };

    byte short_control[NUM_LAYOUT];
    byte short_ch[NUM_LAYOUT] = {16, 16, 16};
    byte short_type[NUM_LAYOUT] = {1, 1, 1};
    byte long_control[NUM_LAYOUT];
    byte long_ch[NUM_LAYOUT] = {16, 16, 16};
    byte long_type[NUM_LAYOUT] = {1, 1, 1};
    byte snap[NUM_LAYOUT] = {0, 0, 0};
    byte short_toggle[NUM_LAYOUT] = {0, 0, 0};
    byte long_toggle[NUM_LAYOUT] = {0, 0, 0};
    bool buttonState = LOW;
    bool held = LOW;
    bool latch = LOW;
    bool ext_MIDI_On = LOW;
    int toggle_state[NUM_LAYOUT] = {0, 0, 0};
    bool lastButtonState = LOW;
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

    void update_button() {
      if (!ext_MIDI_On) {
        // buttonState  = !digitalRead(b_pins[num]);
        int reading  = !digitalRead(b_pins[num]);
        if (reading != lastButtonState) {
          lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay) {
          if (reading != buttonState) {
            buttonState = reading;
          }
        }
        lastButtonState = reading;
      }
      check_button();
    }

    void check_button() {
      if (snap[current_layout]) { // if snap is ON, only short press messages are sent
        if (short_toggle[current_layout] == 1) {
          if (buttonState && !held) {
            held = HIGH;
            if (toggle_state[current_layout] == 0) toggle_state[current_layout] = 127;
            else toggle_state[current_layout] = 0;
            sendMessage(short_type[current_layout], short_control[current_layout], toggle_state[current_layout], short_ch[current_layout]);
            if (num < NUM_LEDS) l[num].led_update(HIGH);
          }
          if (!buttonState && held) {
            held = LOW;
            if (num < NUM_LEDS) l[num].led_update(LOW);
          }
        }
        else {
          if (buttonState && !held) {
            held = HIGH;
            sendMessage(short_type[current_layout], short_control[current_layout], 127, short_ch[current_layout]);
            if (num < NUM_LEDS) l[num].led_update(HIGH);
          }
          if (!buttonState && held) {
            held = LOW;
            sendMessage(short_type[current_layout], short_control[current_layout], 0, short_ch[current_layout]);
            if (num < NUM_LEDS) l[num].led_update(LOW);
          }
        }
      }
      else {
        if (buttonState && !held && !latch) {
          time_pressed = millis();
          held = HIGH;
          if (num < NUM_LEDS) l[num].led_update(HIGH);
        }
        if (held && !buttonState) {
          if (num < NUM_LEDS) l[num].led_update(LOW);
          if (!latch) {
            sendMessage(short_type[current_layout], short_control[current_layout], 127, short_ch[current_layout]);
            sendMessage(short_type[current_layout], short_control[current_layout], 0, short_ch[current_layout]);
          }
          held = LOW;
          latch = LOW;
        }
        if (held && ((millis() - time_pressed) > hold_button_time) && !latch ) {
          if (num < NUM_LEDS) l[num].led_update(LOW);
          if (num < 6) {
          sendMessage(long_type[current_layout], long_control[current_layout], 127, long_ch[current_layout]);
          sendMessage(long_type[current_layout], long_control[current_layout], 0, long_ch[current_layout]);
          }
          latch = HIGH;
        }
      }
    }
    void set_button_on() {
      buttonState = HIGH;
    }
    void set_button_off() {
      buttonState = LOW;
    }
};
Button b[NUM_BUTTONS] = {Button(0), Button(1), Button(2), Button(3), Button(4), Button(5), Button(6), Button(7)};
