byte sysex_data[30];
byte sysex_index;
byte cc_data[3];
bool sysex_in = LOW;
bool cc_in = LOW;
bool pc_in = LOW;
bool noteon_in = LOW;
bool noteoff_in = LOW;
byte inByte;


void sendMessage(byte type, byte control, byte channel, byte value) {
  switch (type) {
    case 0:
      sendUSBNote(control, value, channel);
      senderialNote(control, value, channel);
      break;
    case 1:
      sendUSBControlChange(control, value, channel);
      senderialControlChange(control, value, channel);
      break;
    case 2:
      sendPC(control, channel);
      break;
  }
}

void check_led(byte channel, byte control, byte value) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (l[i].control[current_layout] == control) {
      l[i].set_color(value, channel);
      l[i].led_update(b[i].held);
    }
  }
}


void check_custom_led() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (l[i].control[current_layout] > 128) {
      l[i].set_color(l[i].control[current_layout]-128, 16);
      l[i].led_update(b[i].held);
    }
  }
}

/*void check_layout_led() {
  byte color = 15; //14;
  if (current_layout == 1) color = 21;
  else if (current_layout == 2) color = 9;
  check_led(16, 50, color);
}*/

void eeprom_store(byte rcvd_layout, byte  num, byte  type, byte  control, byte  channel) {
  EEPROM.write(100 * rcvd_layout + num, type);
  EEPROM.write(100 * rcvd_layout + num + 30, control);
  EEPROM.write(100 * rcvd_layout + num + 60, channel);
}

void eeprom_store_snap(byte rcvd_layout, byte  num, byte snap) {
  EEPROM.write(100 * rcvd_layout + num + 18, snap);
}
