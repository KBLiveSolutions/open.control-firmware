long unsigned _now = millis();
long unsigned _now_char = micros();
byte ticker = 0;
byte but = 0;
byte ana = 0;

midiEventPacket_t rx;

void set_led_matrix() {
  DDRC |= 1 << DDC7;
  DDRD |= 1 << DDD7;
  DDRF |= 1 << DDF4;
  DDRD |= 1 << DDD6;
  DDRD |= 1 << DDD4;
  DDRF |= 1 << DDF1;
  DDRF |= 1 << DDF0;
  DDRB |= 1 << DDB4;
}


void set_LEDs() {
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<LED_TYPE , LED_PIN, GRB>(leds, NUM_LEDS); // for GRB LEDs
  FastLED.setBrightness(BRIGHTNESS);
  byte init_led_color_red[NUM_LEDS] = {80, 0, 17, 100, 124, 90};
  byte init_led_color_green[NUM_LEDS] = {0, 78, 23, 100, 49, 0};
  byte init_led_color_blue[NUM_LEDS] = {127, 46, 80, 0, 0, 12};
  for ( byte i = 0; i < NUM_LEDS; i += 1) {
    l[i].r = init_led_color_red[i];
    l[i].g = init_led_color_green[i];
    l[i].b = init_led_color_blue[i];
    l[i].led_update(LOW);
  }
}
void clear_EEPROM() {
  for (byte i = 0; i < 350; i++)
    EEPROM.write(i, 255);

}

void set_EEPROM() {
  byte default_short[3][6] = {{13, 1, 2, 14, 15, 50}, {22, 24, 26, 18, 19, 50}, {1, 6, 7, 9, 8, 50}};
  byte default_long [3][6] = {{0, 17, 3, 0, 0, 51}, {0, 0, 0, 0, 0, 51}, {0, 0, 0, 0, 0, 51}};
  byte default_leds [3][6] = {{13, 1, 2, 14, 15, 50}, {22, 24, 26, 18, 19, 50}, {1, 6, 7, 9, 8, 50}};
  byte default_sliders [3][2] = {{73, 59}, {61, 62}, {1, 6}};
  byte default_display [3] = {0, 1, 4};

  for (byte layout_num = 0; layout_num < NUM_LAYOUT; layout_num++) {
    for (byte i = 0; i <= 22; i++) { // Retrieve all EEPROM
      byte type = EEPROM.read(layout_num * 100 + i);
      byte control = EEPROM.read(layout_num * 100 + i + 30);
      byte channel = EEPROM.read(layout_num * 100 + i + 60);
      if (i < 6 ) { // Retrieve Short Button
        b[i].short_control[layout_num] = default_short[layout_num][i];
        if (control != 255) {
          b[i].short_control[layout_num] = control;
          b[i].short_ch[layout_num] = channel;
          b[i].short_type[layout_num] = type;
        }
      }
      if (6 <= i && i < 12) { // Retrieve Long Button
        b[i - 6].long_control[layout_num] = default_long[layout_num][i - 6];
        if (control != 255) {
          b[i - 6].long_control[layout_num] = control;
          b[i - 6].long_ch[layout_num] = channel;
          b[i - 6].long_type[layout_num] = type;
        }
      }
      if (12 <= i && i < 18) { // Retrieve LEDs
        l[i - 12].control[layout_num] = default_leds[layout_num][i - 12];
        if (control != 255) {
          l[i - 12].control[layout_num] = control;
        }
      }
      if (18 <= i && i < 20) { // Retrieve Sliders
        a[i - 18].control[layout_num] = default_sliders[layout_num][i - 18];
        if (control != 255) {
          a[i - 18].control[layout_num] = control;
          a[i - 18].channel[layout_num] = channel;
        }
      }
      if (i == 20) { // Retrieve Display
        disp.layout[layout_num] = default_display[layout_num];
        if (control != 255) {
          disp.layout[layout_num] = control;
        }
      }
    }
    for (byte i = 0; i < NUM_BUTTONS; i++) { // Retrieve buttons snap
      byte value = EEPROM.read(layout_num * 100 + i + 18);
      if (value != 255) {
        b[i].snap[layout_num] = value;
      }
    }
  }

  for (byte i = 0; i < 10; i++) { // Retrieve Options and send them to Live
    byte value = EEPROM.read(300 + i);
    if (value != 255) {
      byte sysex_array[8] = {240, 122, 29, 1, 19, 30 + i, value, 247};
      sendUSBSysEx(sysex_array, 8);
    }
  }

  for (byte i = 0; i < (NUM_BUTTONS + NUM_SLIDERS); i++) { // Retrieve External MIDI
    byte type = EEPROM.read(276 + i);
    if (type != 255) {
      external_MIDI_type[i] = type;
    }
    byte control = EEPROM.read(284 + i);
    if (EEPROM.read(280 + i) != 255) {
      external_MIDI_control[i] = control;
    }
    byte channel = EEPROM.read(292 + i);
    if (EEPROM.read(280 + i) != 255) {
      external_MIDI_channel[i] = channel;
    }
  }

  if (EEPROM.read(310) != 255) BRIGHTNESS = EEPROM.read(298); // Retrieve LED Brightness
  if (EEPROM.read(311) != 255) matrix_brightness = EEPROM.read(299); // Retrieve Display Brightness
}

void setup() {
  //  clear_EEPROM();
  set_LEDs();
  set_EEPROM();
  set_led_matrix();
  disp.build_text(5);
  Serial1.begin(31250);
}

void loop() {

  rx = MidiUSB.read();
  if (rx.header) onUSBMIDIPacketReceived(rx.header, rx.byte1, rx.byte2, rx.byte3);
  if (Serial1.available()) onSerialByteReceived( Serial1.read());

  b[but].update_button();
  a[ana].check_pot();
  //disp.show_char(ticker);

  but ++;
  ana ++;
 // ticker++;

  if (but == NUM_BUTTONS) but = 0;
  if (ana == NUM_SLIDERS) ana = 0;
  if (ticker == 32) ticker = 0;
  
  if (micros() - _now_char > 477) {
    disp.show_char(ticker);
    _now_char = micros();
  ticker++;
  }
  
  if (millis() - _now > blinking_speed / 2) {
    disp.inc_scroll();
    for (byte i = 0; i < NUM_LEDS; i++) {
      l[i].toggle_led();
      _now = millis();
    }
  }
}
