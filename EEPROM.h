
void setup_EEPROM() {
  EEPROM.begin(1024);
  byte _byte;
  for (byte layout_num = 0; layout_num < NUM_LAYOUT; layout_num++) {
    for (byte i = 0; i < NUM_BUTTONS; i++) {
      _byte = EEPROM.read(layout_num * 100 + i);
      if (_byte != 255) b[i].short_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 8 + i);
      if (_byte != 255) b[i].short_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 16 + i);
      if (_byte != 255) b[i].short_ch[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 24 + i);
      if (_byte != 255) b[i].short_toggle[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 32 + i);
      if (_byte != 255) b[i].snap[layout_num] = _byte;
      
      _byte = EEPROM.read(layout_num * 100 + 40 + i);
      if (_byte != 255) b[i].long_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 48 + i);
      if (_byte != 255) b[i].long_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 56 + i);
      if (_byte != 255) b[i].long_ch[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 64 + i);
      if (_byte != 255) b[i].long_toggle[layout_num] = _byte;
      
      _byte = EEPROM.read(layout_num * 100 + 364 + i);
      if (_byte != 255) b[i].double_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 372 + i);
      if (_byte != 255) b[i].double_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 380 + i);
      if (_byte != 255) b[i].double_ch[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 388 + i);
      if (_byte != 255) b[i].double_toggle[layout_num] = _byte;
    }

    for (byte i = 0; i < NUM_LEDS; i++) {
      // l[i].led_control[layout_num] = default_leds[layout_num][i];
      _byte = EEPROM.read(layout_num * 100 + 72 + i);
      if (_byte != 255) l[i].led_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 78 + i);
      if (_byte != 255) l[i].led_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 84 + i);
      if (_byte != 255) l[i].led_channel[layout_num] = _byte;
    }

    for (byte i = 0; i < NUM_ENCODERS; i++) {
      _byte = EEPROM.read(layout_num * 100 + 90 + i);
      if (_byte != 255) r[i].control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 92 + i);
      if (_byte != 255) r[i].control_hold[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 94 + i);
      if (_byte != 255) r[i].channel[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 96 + i);
      if (_byte != 255) r[i].channel_hold[layout_num] = _byte;
    }

    for (byte i = 0; i < NUM_SLIDERS; i++) {
      _byte = EEPROM.read(layout_num * 100 + 98 + i);
      if (_byte != 255) a[i].control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 100 + i);
      if (_byte != 255) a[i].channel[layout_num] = _byte;
    }
    _byte = EEPROM.read(layout_num * 100 + 102);
    if (_byte != 255) disp.layout[layout_num] = _byte;
  }

  for (byte i = 0; i < 2; i++) {
    // Retrieve Options and send them to Live
    _byte = EEPROM.read(350 + i);
    if (_byte != 255) options[i] = _byte;
  }

  for (byte i = 0; i < 10; i++) {
    // Retrieve External MIDI
    _byte = EEPROM.read(310 + i);
    if (_byte != 255) external_MIDI_type[i] = _byte;
    _byte = EEPROM.read(320 + i);
    if (_byte != 255) external_MIDI_control[i] = _byte;
    _byte = EEPROM.read(330 + i);
    if (_byte != 255) external_MIDI_channel[i] = _byte;
  }

  _byte = EEPROM.read(340);
  if (_byte != 255) matrix_brightness = _byte;  // Retrieve Display Brightness
  _byte = EEPROM.read(341);
  if (_byte != 255) BRIGHTNESS = _byte;  // Retrieve LED Brightness
  pixels.setBrightness(BRIGHTNESS);
  _byte = EEPROM.read(342);
  if (_byte != 255) USB_thru = _byte;  // Retrieve USB_thru
  _byte = EEPROM.read(343);
  if (_byte != 255) SERIAL_thru = _byte;  // Retrieve SERIAL_thru

  for (byte i = 0; i < NUM_SLIDERS; i++) {
    _byte = EEPROM.read(360 + i);
    if (_byte != 255) a[i].pedal_min = _byte;
    _byte = EEPROM.read(362 + i);
    if (_byte != 255) a[i].pedal_max = _byte;
  }
}

void clear_EEPROM() {
  EEPROM.begin(512);
  for (byte i = 0; i < 350; i++) {
    EEPROM.write(i, 255);
    EEPROM.commit();
  }
}
