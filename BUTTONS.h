//////////////////////////////
// BUTTONS
//////////////////////////////

// const int b_pins[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, 13, 14};    // Pin number for buttons 1 to 8 (buttons 7 & 8 are the Encoders buttons)

const int b_pins[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, 13, 14};    // Pin number for buttons 1 to 8 (buttons 7 & 8 are the Encoders buttons)

class NewButton : public OneButton {
  private:
    int pin;
    int number;
  public:
    NewButton(int num, uint8_t pin) : OneButton(pin, true) {
      pin = pin;
      number = num;
      for (int i = 0 ; i < NUM_LAYOUT ; i++) {  // Sets button default values
        short_control[i] = default_short[i][num];
        long_control[i] = default_long[i][num];
        double_control[i] = default_double[i][num];
      }
    }

    bool btn_state = LOW;
    bool btn_latch = LOW;
    bool ext_MIDI_On = LOW;
    int snap[NUM_LAYOUT] = {0, 0, 0};
    byte short_control[NUM_LAYOUT];
    byte short_ch[NUM_LAYOUT] = {16, 16, 16};
    byte short_type[NUM_LAYOUT] = {1, 1, 1};
    byte short_toggle[NUM_LAYOUT] = {0, 0, 0};
    byte long_control[NUM_LAYOUT];
    byte long_ch[NUM_LAYOUT] = {16, 16, 16};
    byte long_type[NUM_LAYOUT] = {1, 1, 1};
    byte long_toggle[NUM_LAYOUT] = {0, 0, 0};
    byte double_control[NUM_LAYOUT];
    byte double_ch[NUM_LAYOUT] = {16, 16, 16};
    byte double_type[NUM_LAYOUT] = {1, 1, 1};
    byte double_toggle[NUM_LAYOUT] = {0, 0, 0};
    byte short_value[NUM_LAYOUT] = {0, 0, 0};
    byte long_value[NUM_LAYOUT] = {0, 0, 0};
    byte double_value[NUM_LAYOUT] = {0, 0, 0};


    void button_check(bool state) {
      if (state && !btn_state) {
        btn_state = HIGH;
        change_led(HIGH);
        if (snap[current_layout] > 0) sendMessage(short_type[current_layout], short_control[current_layout], 127, short_ch[current_layout]);
      }
      if (!state && btn_state)  {
        btn_state = LOW;
        if (snap[current_layout] > 0) {
          sendMessage(short_type[current_layout], short_control[current_layout], 0, short_ch[current_layout]);
          change_led(LOW);
        }
      }
    }

    void simpleClick() {
      if (snap[current_layout] == 0) {
        if (short_toggle[current_layout] == 0) {
          sendMessage(short_type[current_layout], short_control[current_layout], 127, short_ch[current_layout]);
          sendMessage(short_type[current_layout], short_control[current_layout], 0, short_ch[current_layout]);
        }
        else {
          if (short_value[current_layout] == 0) short_value[current_layout] = 127;
          else short_value[current_layout] = 0;
          sendMessage(short_type[current_layout], short_control[current_layout], short_value[current_layout], short_ch[current_layout]);
        }
        change_led(LOW);
      }
    }

    void longClick() {
      if (number < 6 and snap[current_layout] == 0) {
        if (long_toggle[current_layout] == 0) {
          sendMessage(long_type[current_layout], long_control[current_layout], 127, long_ch[current_layout]);
          sendMessage(long_type[current_layout], long_control[current_layout], 0, long_ch[current_layout]);
        }
        else {
          if (long_value[current_layout] == 0) long_value[current_layout] = 127;
          else long_value[current_layout] = 0;
          sendMessage(long_type[current_layout], long_control[current_layout], long_value[current_layout], long_ch[current_layout]);
        }
        change_led(LOW);
      }
    }

    void doubleClick() {
      if (snap[current_layout] == 0) {
        if (double_toggle[current_layout] == 0) {
          sendMessage(double_type[current_layout], double_control[current_layout], 127, double_ch[current_layout]);
          sendMessage(double_type[current_layout], double_control[current_layout], 0, double_ch[current_layout]);
        }
        else {
          if (double_value[current_layout] == 0) double_value[current_layout] = 127;
          else double_value[current_layout] = 0;
          sendMessage(double_type[current_layout], double_control[current_layout], double_value[current_layout], double_ch[current_layout]);
        }
        change_led(LOW);
      }
    }


    void change_led(bool state) {
      if (number < 6) l[number].led_update(state);
    }

    void attachSimple(parameterizedCallbackFunction f) {
      OneButton::attachClick(f, this);
    }

    void attachDouble(parameterizedCallbackFunction f) {
      OneButton::attachDoubleClick(f, this);
    }

    void attachLong(parameterizedCallbackFunction f) {
      OneButton::attachLongPressStart(f, this);
    }

};


NewButton b[NUM_BUTTONS] = {NewButton(0, b_pins[0]), NewButton(1, b_pins[1]), NewButton(2, b_pins[2]), NewButton(3, b_pins[3]), NewButton(4, b_pins[4]), NewButton(5, b_pins[5]), NewButton(6, b_pins[6]), NewButton(7, b_pins[7])};

void simpleClickcallBack(void *p) {
  ((NewButton *) p)->simpleClick();
}

void longClickcallBack(void *p) {
  ((NewButton *) p)->longClick();
}

void doubleClickcallBack(void *p) {
  ((NewButton *) p)->doubleClick();
}

void setup_Buttons(){
  for (int i=0; i<NUM_BUTTONS; i++){
  b[i].attachSimple(simpleClickcallBack);
  b[i].attachLong(longClickcallBack);
  b[i].attachDouble(doubleClickcallBack);
  b[i].setClickTicks(150);
  b[i].setDebounceTicks(20);
  }
}
