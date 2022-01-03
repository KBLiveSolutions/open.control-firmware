
// =============   MIDI SEND =================

void sendSerialControlChange(byte control, byte value, byte channel) {
  SERIAL_MIDI.sendControlChange(control, value, channel);
}

void sendSerialNote(byte note, byte vel, byte channel) {
  SERIAL_MIDI.sendNoteOn(note, vel, channel);
}

void sendSerialPC(byte pc, byte channel) {
}

void sendSerialSysEx(const uint8_t *data, int _size) {
  SERIAL_MIDI.sendSysEx(_size, data, true);
}

// =============  EXTERNAL MIDI  =================


void onExternalMessageReceived(byte channel, byte control, byte value, byte type) {
<<<<<<< Updated upstream
=======
  USB_MIDI.sendNoteOn(control, value, channel);
>>>>>>> Stashed changes
  bool is_Control = LOW;
  for (byte i = 0; i < NUM_BUTTONS; i++) {  // goes through each Button
    if (channel == external_MIDI_channel[i] && control == external_MIDI_control[i]  && type == external_MIDI_type[i]) { // checks if the message received is an externem_MIDI control/channel
      is_Control = HIGH;
      if (value > 0) {
      b[i].tick(HIGH);
      //  b[i].set_button_on();
      }
      else {
      b[i].tick(LOW);
      //  b[i].ext_MIDI_On = LOW;
      //  b[i].set_button_off();
      }
    }
  }
  for (byte i = 0; i < NUM_SLIDERS; i++) {
    is_Control = HIGH;
    if (channel == external_MIDI_channel[i + NUM_BUTTONS] && control == external_MIDI_control[i + NUM_BUTTONS]) {
      sendUSBControlChange(a[i].control[current_layout], value, a[i].channel[current_layout]);
    }
  }
  if (!is_Control) {
    // if (type == 0) sendSerialNote(control, value, channel);
    // if (type == 1) sendSerialControlChange(control, value, channel);
    // if (type == 2) sendSerialPC(control, channel);
  }
}

void onSerialControlChange(byte channel, byte control, byte value) {
  if (channel > 13) { // if MIDI In is used as Remote
    check_led(channel, control, value, HIGH);
    check_rotary(channel, control, value);
    check_slider(channel, control, value);
  }
  else onExternalMessageReceived(channel, control, value, 1); // if MIDI In is used as External MIDI
  // if (USB_thru > 0) USB_MIDI.sendControlChange(control, value, channel);
  // if (SERIAL_thru > 0) SERIAL_MIDI.sendControlChange(control, value, channel);
}

void onSerialNoteOn(byte channel, byte note, byte vel) {
  onExternalMessageReceived(channel, note, vel, 0);
  if (USB_thru > 0) USB_MIDI.sendNoteOn(note, vel, channel);
  if (SERIAL_thru > 0) SERIAL_MIDI.sendNoteOn(note, vel, channel);
}


void onSerialSysEx(uint8_t *data, unsigned int _length) {
  if (data[1] == 122 && data[2] == 29 && data[3] == 1 && data[4] == 19) {
    switch (data[5]) {

      case 2:
        { // Handshake with Live
          byte sysexArrayBoot[] = { 240, 122, 29, 1, 19, 2, 247 };  //String that answers to the MIDI Remote Script for Ableton Live
          sendSerialSysEx(sysexArrayBoot, 7);
        }
        break;

      case 3: {   // Disconnect message received
          int disconnect_text[5] = { 13, 13, 13, 13, 13};
          disp.build_text(5, disconnect_text);
          init_LEDS();
        }
        break;


      // receive Data from Live

      case 40:
        { // Layout Value received
          clear_leds();
          disp.clear_text();
          current_layout = data[6];
          int page_text[MAX_CHAR] = { 48, 65, 71, 69, 0, (current_layout + 17) };
          disp.build_text(6, page_text);
          showing_page = HIGH;
          _now_page = millis();
          check_custom_led();
        }
        break;

      case 51:
        { // Text received
          byte text_len = data[7];  //min(MAX_CHAR, data[7]);
          if (data[6] == disp.layout[current_layout]) {
            disp.text_len = data[7];
            for (byte i = 0; i < text_len; i++) {
              disp.data_text[i] = data[8 + i];
            }
          }
          if (!showing_page) disp.build_text(disp.text_len, disp.data_text);
        }
        break;

      case 54:
        { // Direct Text received
          byte text_len = data[7];
          for (byte i = 0; i < text_len; i++) {
            disp.temp_text[i] = data[8 + i];
          }
          disp.build_text(text_len, disp.temp_text);
          showing_page = HIGH;
          _now_page = millis();
        }
        break;

      case 52:
        { // Receive Looper number from Live
          byte note = data[6];
          byte type = data[7];
          byte chnl = data[8];

          for (byte layout = 0; layout < NUM_LAYOUT; layout++) {
            for (byte i = 0; i < NUM_BUTTONS; i++) {
              if (b[i].short_ch[layout] == chnl && b[i].short_type[layout] == type) {
                b[i].short_control[layout] = note;
              }
              if (b[i].long_ch[layout] == chnl && b[i].long_type[layout] == type) {
                b[i].long_control[layout] = note;
              }
            }
          }
        }
        break;

      case 53:
        { // Receive Looper Clear All
          byte note = data[6];
          byte type = data[7];
          byte chnl = data[8];
          sendSerialNote(note, 127, chnl);
          sendSerialNote(note, 0, chnl);
        }
        break;
    }
  }
}
