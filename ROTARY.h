
#include <RotaryEncoder.h>

int rotA_pin[2] = {12, 10};
int rotB_pin[2] = {11, 9};

class Rotary {
  private:
    RotaryEncoder encoder;
    byte number;
    int pin_A;
    int pin_B;
    int pos = 0;
    byte default_rotary [3][2] = {{59, 60}, {61, 62}, {63, 64}};
    byte default_rotary_hold [3][2] = {{89, 90}, {73, 37}, {99, 88}};

  public:
    Rotary(int num) : encoder(rotA_pin[num], rotB_pin[num], RotaryEncoder::LatchMode::FOUR3) {
      number = num;
      for (int i = 0 ; i < NUM_LAYOUT ; i++) {
        control[i] = default_rotary[i][number];
        channel[i] = 16;
        control_hold[i] = default_rotary_hold[i][number];
        channel_hold[i] = 16;
      }
    }
    byte control[NUM_LAYOUT];
    byte channel[NUM_LAYOUT];
    byte control_hold[NUM_LAYOUT];
    byte channel_hold[NUM_LAYOUT];
    int _value = 64;
    int _value_hold = 64;
    long unsigned _now = millis();

    void update_rotary()
    {
      encoder.tick();
      int newPos = encoder.getPosition();

      if (pos != newPos) {
        if(b[6+number].held) {
          int newValue = _value_hold;
          b[6+number].latch = HIGH;
          if (int(encoder.getDirection()) > 0) {
              if ((millis() - _now) < 25) _value_hold = _value_hold + 4;
              else _value_hold++;
            }
            else {
              if ((millis() - _now) < 25) _value_hold = _value_hold - 4;
              else _value_hold--;
            }
            _value_hold = constrain(_value_hold, 0, 127);
            if (_value_hold != newValue) {
              USB_MIDI.sendControlChange(control_hold[current_layout], _value_hold, channel_hold[current_layout]);
              SERIAL_MIDI.sendControlChange(control_hold[current_layout], _value_hold, channel_hold[current_layout]);
            }
        }
        
        else {
          int newValue = _value;
          if (int(encoder.getDirection()) > 0) {
            if ((millis() - _now) < 25) _value = _value + 4;
            else _value++;
          }
          else {
            if ((millis() - _now) < 25) _value = _value - 4;
            else _value--;
          }
          _value = constrain(_value, 0, 127);
          if (_value != newValue) {
            USB_MIDI.sendControlChange(control[current_layout], _value, channel[current_layout]);
            SERIAL_MIDI.sendControlChange(control[current_layout], _value, channel[current_layout]);
          }
        }
        pos = newPos;
        _now = millis();
      }
    };
}

r[2] = {Rotary(0), Rotary(1)};
