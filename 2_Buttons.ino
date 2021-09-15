//////////////////////////////
// BUTTONS
//////////////////////////////

class Button {
  private:
    unsigned long time_pressed = 0;
    Bounce bouncer = Bounce( 0, 25 );
  public:
    byte num;
    Button(byte number) {
      num = number;
      bouncer = Bounce( b_pins[num], 25 );
      pinMode(b_pins[num], INPUT_PULLUP);
    };

    byte short_control[NUM_LAYOUT] = {21, 2, 3};
    byte short_ch[NUM_LAYOUT] = {16, 16, 16};
    byte short_type[NUM_LAYOUT] = {1, 1, 1};
    byte long_control[NUM_LAYOUT] = {31, 4, 5};
    byte long_ch[NUM_LAYOUT] = {16, 16, 16};
    byte long_type[NUM_LAYOUT] = {1, 1, 1};
    byte snap[NUM_LAYOUT] = {0, 0, 0};
    bool state = LOW;
    bool held = LOW;
    bool latch = LOW;
    bool ext_MIDI_On = LOW;

    void update_button() {
      if (!ext_MIDI_On) {
        bouncer.update();
        state = !bouncer.read();
      }
      check_button();
    }

    void check_button() {
      if (snap[current_layout]) {
        if (state && !held) {
          held = HIGH;
          l[num].led_update(HIGH);
          sendMessage(short_type[current_layout], short_control[current_layout], short_ch[current_layout], 127);
        }
        if (!state && held) {
          held = LOW;
          l[num].led_update(LOW);
          sendMessage(short_type[current_layout], short_control[current_layout], short_ch[current_layout], 0);
        }
      }
      else {
        if (state && !held && !latch) {
          time_pressed = millis();
          held = HIGH;
          l[num].led_update(HIGH);
        }
        if (held && !state) {
          l[num].led_update(LOW);
          if (!latch) {
            sendMessage(short_type[current_layout], short_control[current_layout], short_ch[current_layout], 127);
            sendMessage(short_type[current_layout], short_control[current_layout], short_ch[current_layout], 0);
          }
          held = LOW;
          latch = LOW;
        }
        if (held && ((millis() - time_pressed) > hold_button_time) && !latch) {
          l[num].led_update(LOW);
            sendMessage(long_type[current_layout], long_control[current_layout], long_ch[current_layout], 127);
            sendMessage(long_type[current_layout], long_control[current_layout], long_ch[current_layout], 0);
          latch = HIGH;
        }
      }
    }
    void set_button_on() {
      state = HIGH;
    }
    void set_button_off() {
      state = LOW;
    }
};

Button b[NUM_BUTTONS] = {Button(0), Button(1), Button(2), Button(3), Button(4), Button(5)};
