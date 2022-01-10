
bool USB_clock;
byte USB_thru = 0;
byte SERIAL_thru = 0;

byte external_MIDI_control[NUM_BUTTONS + NUM_SLIDERS] = {1, 2, 3, 4, 5, 6, 1, 1};
byte external_MIDI_channel[NUM_BUTTONS + NUM_SLIDERS] = {1, 1, 1, 1, 1, 1, 1, 1};
byte external_MIDI_type[NUM_BUTTONS + NUM_SLIDERS] = {0, 0, 0, 0, 0, 0, 0, 0};


void sendMessage(byte type, byte control, byte value, byte channel) {
  switch (type) {
    case 0:
      USB_MIDI.sendNoteOn(control, value, channel);
      SERIAL_MIDI.sendNoteOn(control, value, channel);
      break;
    case 1:
      USB_MIDI.sendControlChange(control, value, channel);
      SERIAL_MIDI.sendControlChange(control, value, channel);
      break;
    case 2:
    if (value == 127){
      USB_MIDI.sendProgramChange(control, channel);
      SERIAL_MIDI.sendProgramChange(control, channel);
    }
      break;
  }
}

// BLINK

void clock_received() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    if (l[i].led_channel[current_layout] == 15) l[i].blink_slow(_clock);
    else if (l[i].led_channel[current_layout] == 14) l[i].blink_fast(_clock);
  }
  _clock += 1;
  if (_clock == 4 ) _clock = 0;
}

// FADE

void clock_continue() {
  _clock = 0;
}

void clock_start() {
  _clock = 0;
  for (byte i = 0; i < NUM_LEDS; i++) {
    l[i].show_color();
    delay(1);
  }
}

void clock_stop() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    l[i].show_color();
    delay(1);
  }
}

void clock_start_serial() {
  _clock = 0;
  for (byte i = 0; i < NUM_LEDS; i++) {
    l[i].show_color();
  }
}

void clear_leds() {
  for (byte i = 0; i < NUM_LEDS; ++i) {
    l[i].led_off();
    l[i].r = 0;
    l[i].g = 0;
    l[i].b = 0;
  }
}

void eeprom_store(int rcvd_layout, int _offset, int value) {
  EEPROM.write(100 * rcvd_layout + _offset, value);
  EEPROM.commit();
}

void raw_eeprom_store(int position, int  value) {
  EEPROM.write(position, value);
  EEPROM.commit();
}
