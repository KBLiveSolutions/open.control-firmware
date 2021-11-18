
//////////////////////////////
// ANALOG
//////////////////////////////

const int a_pins[NUM_SLIDERS] = {A1, A0};

class AnalogPot {

  private:
    int pin;
    static const int RunningAverageCount = 8;
    float RunningAverageBuffer[RunningAverageCount];
    int NextRunningAverage;
    byte default_sliders [3][2] = {{73, 59}, {61, 62}, {61, 62}};

  public:
    AnalogPot(int _pin) {
      pin = _pin;
    }
    byte control[NUM_LAYOUT] = {default_sliders[0][0], default_sliders[1][0], default_sliders[2][0]};
    byte channel[NUM_LAYOUT] = {16, 16, 16};
    byte valuePot;
    byte lastValuePot;
    void check_pot() {
      int _value = analogRead(pin);
      valuePot = map(average(_value), 0, 4096, 0, 128);
      valuePot = constrain( valuePot, 0, 127);

      if ((valuePot != lastValuePot)) {
        lastValuePot = valuePot;
        if (b[5].held == HIGH) {
          if (pin == a_pins[1]) {
            //            BRIGHTNESS = map( analog_read.getValue(), 0, 1024, 30, 255);
            //            FastLED.setBrightness(BRIGHTNESS);
            //            FastLED.show();
            //            EEPROM.write(298, BRIGHTNESS);

          }
          if (pin == a_pins[0]) {
            // matrix_brightness =  map(analog_read.getValue(), 0, 1024, 30, 0);
            EEPROM.write(299, matrix_brightness);
          }
        }
        else {
          USB_MIDI.sendControlChange(control[current_layout], valuePot, channel[current_layout]);
          SERIAL_MIDI.sendControlChange(control[current_layout], valuePot, channel[current_layout]);
        }
      }
    }

    int average(int value)
    {
      float RawValue = value;
      RunningAverageBuffer[NextRunningAverage++] = RawValue;
      if (NextRunningAverage >= RunningAverageCount)
      {
        NextRunningAverage = 0;
      }
      float RunningAverageValue = 0;
      for (int i = 0; i < RunningAverageCount; ++i)
      {
        RunningAverageValue += RunningAverageBuffer[i];
      }
      RunningAverageValue /= RunningAverageCount;
      return RunningAverageValue;
    }
};

AnalogPot a[NUM_SLIDERS] = {AnalogPot(a_pins[0]), AnalogPot(a_pins[1])};
