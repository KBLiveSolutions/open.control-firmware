
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

  public:
    Rotary(int num) : encoder(rotA_pin[num], rotB_pin[num], RotaryEncoder::LatchMode::FOUR3) {
      number = num;
      for (int i = 0 ; i < NUM_LAYOUT ; i++) {
        control[i] = default_rotary[i][number];
        channel[i] = 16;
      }
    }
    byte control[NUM_LAYOUT];
    byte channel[NUM_LAYOUT];
    int _value = 64;
    long unsigned _now = millis();

    void update_rotary()
    {
      encoder.tick();

      int newPos = encoder.getPosition();
      int newValue = _value;
      if (pos != newPos) {
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
        pos = newPos;
        _now = millis();
      }
    };
}

r[2] = {Rotary(0), Rotary(1)};
