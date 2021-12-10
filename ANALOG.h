
//////////////////////////////
// ANALOG
//////////////////////////////


Hysteresis <uint8_t> pot_hysteresis(5);

RPI_PICO_Timer Analog_Hysteresis_Timer(0);
#define Analog_Hysteresis_Timer_TIMER_INTERVAL 100000

bool Analog_Hysteresis_Timer_Handler(struct repeating_timer *t) {
  Analog_Hysteresis_Timer.stopTimer();
  pedal_state[0] = LOW;
  pedal_state[1] = LOW;
  return true;
}


const int a_pins[NUM_SLIDERS] = {27, 26};

class AnalogPot {

  private:
    int number;
    int pin;
    static const int RunningAverageCount = 20;
    float RunningAverageBuffer[RunningAverageCount];
    int NextRunningAverage;
    byte default_sliders [3][2] = {{73, 59}, {61, 62}, {61, 62}};
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
    int PotPrevVal = 0; // needs global scope

    int analog_to_MIDI(int analog_in) {
      int _val = constrain( map(analog_in, pedal_min, pedal_max, 0, 128), 0, 127);
      return _val;
    }

    void check_pot() {
      if (pedal_state[0] && pedal_state[1]) {
        pedal_state[0] = LOW;
        pedal_state[1] = LOW;
      }
      
      int _value = analogRead(pin) / 16;
      value_MIDI = average(constrain( map(_value, pedal_min, pedal_max, 0, 128), 0, 127));

      if (pedal_state[number] ) {
        //if((lastValue != value_MIDI)) {
        if (!pedal_state[other_pedal] && (lastValue != value_MIDI)) {
          lastValue = value_MIDI;
          USB_MIDI.sendControlChange(control[current_layout], int(value_MIDI), channel[current_layout]);
          SERIAL_MIDI.sendControlChange(control[current_layout], int(value_MIDI), channel[current_layout]);
          Analog_Hysteresis_Timer.restartTimer();
        }
      }
      else {
        if (lastValue + 1 < value_MIDI ||  value_MIDI < lastValue - 1) {
          lastValue = value_MIDI;
          pedal_state[number] = HIGH;
          USB_MIDI.sendControlChange(control[current_layout], int(value_MIDI), channel[current_layout]);
          SERIAL_MIDI.sendControlChange(control[current_layout], int(value_MIDI), channel[current_layout]);
        }
      }
    }

    void _calibrate( int num, int min_or_max) {
      int value = analogRead(pin) / 16;
      value = constrain(value, 20, 235);
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
