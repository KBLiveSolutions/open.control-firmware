
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
  bool is_Control = LOW;
  for (byte i = 0; i < NUM_BUTTONS; i++) {  // goes through each Button
    if (channel == external_MIDI_channel[i] && control == external_MIDI_control[i]  && type == external_MIDI_type[i]) { // checks if the message received is an externem_MIDI control/channel
      is_Control = HIGH;
      if (value > 0) {
        b[i].ext_MIDI_On = HIGH;
        b[i].set_button_on();
      }
      else {
        b[i].ext_MIDI_On = LOW;
        b[i].set_button_off();
      }
    }
  }
  for (byte i = 0; i < NUM_SLIDERS; i++) {
    is_Control = HIGH;
    if (channel == external_MIDI_channel[i+NUM_BUTTONS] && control == external_MIDI_control[i+NUM_BUTTONS]) {
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
  // if MIDI In is used as Remote
  if (channel == 16) {
    check_led(channel, control, value, HIGH);
    check_rotary(channel, control, value);
  }
  // if MIDI In is used as External MIDI
  else onExternalMessageReceived(channel, control, value, 1);
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
    if (data[5] < 30) {
      switch (data[5]) {

        // Connect Disconnect

        case 1: {    // Handshake with Editor
            byte sysexArrayBoot[] = {240, 122, 29, 1, 19, 68, 1, 0, 247};  //String that answers to the MIDI Remote Script for Ableton Live
            sendSerialSysEx(sysexArrayBoot, 9);
          }
          break;

        case 2: {    // Handshake with Live
            byte sysexArrayBoot[] = {240, 122, 29, 1, 19, 2, 247};  //String that answers to the MIDI Remote Script for Ableton Live
            sendSerialSysEx(sysexArrayBoot, 7);
            /*     for (byte i = 0; i < 10; i++) { // sending the options
                   byte option = EEPROM.read(300 + i);
                   if (option != 255) {
                     byte sysex_array[8] = {240, 122, 29, 1, 19, 30 + i, option, 247};
                     sendSerialSysEx(sysex_array, 8);
                   }
                 }*/
          }
          break;

        case 3: {   // Disconnect message received
            for (byte i = 0; i < 5 ; i++) {
              disp.data_text[i] = 13;
            }
            disp.text_len = 5;
            disp.build_data_text();
            init_LEDS();
          }
          break;


        case 20: {   // Layout Value received
            clear_leds();
            show_page_number = HIGH;
            disp.clear_text();
            current_layout = data[6];
            disp.build_page_text(data[6] + 17);
            Show_Page_Timer.restartTimer();
            check_custom_led();
          }
          break;

        case 21: {   // Text received
            byte text_len = data[7]; //min(MAX_CHAR, data[7]);
            if (data[6] == disp.layout[current_layout]) {
              disp.text_len = data[7];
              for (byte i = 0; i < text_len ; i++) {
                disp.data_text[i] =  data[8 + i];
              }
            }
            if (!show_page_number) disp.build_data_text();
          }
          break;

        case 22: {   // Receive Looper number from Live
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

        case 23: {   // Receive Looper Clear All
            byte note = data[6];
            byte type = data[7];
            byte chnl = data[8];
            sendUSBNote(note, 127, chnl);
            sendUSBNote(note, 0, chnl);
          }
          break;


      }
    }
  }
}
