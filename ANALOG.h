
//////////////////////////////
// ANALOG
//////////////////////////////


const int a_pins[NUM_SLIDERS] = {A1, A0};

class AnalogPot {

  private:
    int number;
    int pin;
    static const int RunningAverageCount = 30;
    float RunningAverageBuffer[RunningAverageCount];
    int NextRunningAverage;
    int other_pedal;

  public:
    AnalogPot(int num) {
      number = num;
      pin = a_pins[number];
      if (number == 0) other_pedal = 1;
      else other_pedal = 0;
    }
    byte control[NUM_LAYOUT] = {default_sliders[0][0], default_sliders[1][0], default_sliders[2][0]};
    byte channel[NUM_LAYOUT] = {16, 16, 16};
    int value_MIDI = 0;
    byte lastValue = 0;
    int pedal_min = 0;
    int pedal_max = 255;
    int lastReading = 0;
    int lastMIDIValue = 0;
    int margin = 1;
    int _value = 0;
    bool slider_state = LOW;
    unsigned long lastDebounceTime = 10;  // the last time the output pin was toggled
    unsigned long slider_on_time = 0;  // the last time the output pin was toggled

    unsigned long debounceDelayAnalog = 10; // buttons deouncing time

    int analog_to_MIDI(int analog_in) {
      int _val = constrain( map(analog_in, pedal_min, pedal_max, 0, 128), 0, 127);
      return _val;
    }


    void check_pot() {
      if (control[current_layout] > 0) {
        int reading  = average(analogRead(pin) / 16); // reads button pin state
        // Debounces button
        if ((millis() - lastDebounceTime) > debounceDelayAnalog) {
          if (reading > lastValue + margin || reading < lastValue - margin) {
            lastDebounceTime = millis();
            if (!slider_state) {
              if (abs(analog_to_MIDI(lastValue) - _value) < 5) {
                slider_state = HIGH;
                l[4 + number].show_green();
                slider_on_time = millis();
              }
            }
            else {
              if ((millis() - slider_on_time) > 500) l[4 + number].show_color();
              process_analog(analog_to_MIDI(lastValue));
            }
            lastValue = reading;
          }
        }
        if (reading != lastReading) {
          // process_analog(analog_to_MIDI(lastValue));
        }
        lastReading = reading;

      }
    }



    void process_analog(int value_MIDI) {
      if ((lastMIDIValue != value_MIDI)) {
        lastMIDIValue = value_MIDI;
        USB_MIDI.sendControlChange(control[current_layout], value_MIDI, channel[current_layout]);
        SERIAL_MIDI.sendControlChange(control[current_layout], value_MIDI, channel[current_layout]);
      }
    }

    void _calibrate( int num, int min_or_max) {
      int value = analogRead(pin) / 16;
      value = constrain(value, 0, 255);
      if (min_or_max == 0) {
        pedal_min = value;
        raw_eeprom_store(360 + num, value);
      }
      else {
        pedal_max = value;
        raw_eeprom_store(362 + num, value);
      }
    }

    int average(int value) {
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

AnalogPot a[NUM_SLIDERS] = {AnalogPot(0), AnalogPot(1)};
