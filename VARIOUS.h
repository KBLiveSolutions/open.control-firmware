

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

void eeprom_store(int rcvd_layout, int _offset, int value) {
  EEPROM.write(100 * rcvd_layout + _offset, value);
  EEPROM.commit();
}

void raw_eeprom_store(int position, int  value) {
  EEPROM.write(position, value);
  EEPROM.commit();
}
