// 0 -> 5 : Buttons Short
// 6 -> 11 : Buttons Long
// 12 -> 17 : Leds
// 18 -> 21 : Analog

// prefix = {240, 122, 29, 1, 19};
// data[5] :
//           69 -> receive Live acknowledgement
//           70 -> receive Short Buttons Data
//           71 -> receive Long Buttons Data
//           72 -> receive Led Data
//           73 -> receive Analog Data
//           74 -> receive Dump Request
//           75 -> receive Layout change
//           76 -> receive Disconnect
//           77 -> receive Display data
//           80 -> enable/disable metronome blinking
//           81 -> enable session box
//           82 -> receive Display text
//           99 -> raw RGB Data


void onSerialSysEx(const uint8_t *data) { //, unsigned _length, bool complete) {
  if (data[0] == 122 && data[1] == 29 && data[2] == 1 && data[3] == 19) {
    switch (data[4]) {

      case 2: {    // Handshake with Live
          byte sysexArrayBoot[] = {240, 122, 29, 1, 19, 2, 247};  //String that answers to the MIDI Remote Script for Ableton Live
          sendSerialSysex(sysexArrayBoot, 7);
          // check_layout_led();
          for (byte i = 0; i < 2; i++) {
            if (EEPROM.read(300 + i) != 255) {
              byte sysex_array[8] = {240, 122, 29, 1, 19, 30 + i, EEPROM.read(300 + i), 247};
            }
          }
        }
        break;

      case 3: {   // Disconnect message received
          for (byte i = 0; i < 5 ; i++) {
            disp.text[i] = 13;
          }
          disp.build_text(5);
          clear_leds();
        }
        break;

      // receive Input Data from Live

      case 20: {   // Layout Value received
          clear_leds();
          current_layout = data[5];
          // check_layout_led();
        }
        break;

      case 21: {   // Text received
          byte text_len = min(MAX_CHAR, data[6]);
          if (data[5] == disp.layout[current_layout]) {
            disp.clear_text();
            for (byte i = 0; i < text_len ; i++) {
              disp.text[i] =  data[7 + i];
            }
            disp.build_text(text_len);
          }
        }
        break;

      case 22: {   // Receive Looper number from Live
          byte note = data[5];
          byte type = data[6];
          byte chnl = data[7];

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
          byte note = data[5];
          byte type = data[6];
          byte chnl = data[7];
          sendSerialNote(note, 127, chnl);
          sendSerialNote(note, 0, chnl);
        }
        break;
    }
  }
}


// =============   MIDI SEND =================

void sendSerialControlChange(byte control, byte value, byte channel) {
  byte cc_to_send[3] =  {channel + 175,  control,  value};
  Serial1.write(cc_to_send, 3);
}

void sendSerialNote(byte note, byte vel, byte channel) {
  byte note_to_send[3] =  {channel + 143,  note,  vel};
  Serial1.write(note_to_send, 3);
}

void sendSerialPC(byte pc, byte channel) {
  byte pc_to_send[2] =  {channel + 191,  pc};
  Serial1.write(pc_to_send, 2);
}

void sendSerialSysex(byte *data, byte _size) {
  Serial1.write(data, _size); 
}

// =============   MIDI RECEIVE =================

void onSerialByteReceived(byte inByte){
    if (!sysex_in && !cc_in && !noteoff_in && !noteon_in && !pc_in) {
      if (inByte == 240) {
        sysex_in = HIGH;
      }
      else if (inByte >= 128 && inByte < 144) {
        noteoff_in = HIGH;
      }
      else if (inByte >= 144 && inByte < 160) {
        noteon_in = HIGH;
      }
      else if (inByte >= 176 && inByte < 192) {
        cc_in = HIGH;
      }
      else if (inByte >= 192 && inByte < 208) {
        pc_in = HIGH;
      }
    }
    if (sysex_in) {
      Serial1.readBytesUntil(247, sysex_data, 20);
      onSerialSysEx(sysex_data);
      sysex_in = LOW;
    }
    if (noteoff_in) {
      Serial1.readBytesUntil(300, cc_data, 2);
      onSerialNoteOn(inByte - 143, cc_data[0], 0);
      noteoff_in = LOW;
    }
    if (noteon_in) {
      Serial1.readBytesUntil(300, cc_data, 2);
      onSerialNoteOn(inByte - 143, cc_data[0], cc_data[1]);
      noteon_in = LOW;
    }
    if (cc_in) {
      Serial1.readBytesUntil(300, cc_data, 2);
      onSerialControlChange(inByte - 175, cc_data[0], cc_data[1]);
      cc_in = LOW;
    }
    if (pc_in) {
      Serial1.readBytesUntil(300, cc_data, 1);
      sendSerialPC(cc_data[0], inByte - 191);
      pc_in = LOW;
    }
}

void onSerialControlChange(byte channel, byte control, byte value) {
  if (channel > 13) check_led(channel, control, value);
  else onExternalMessageReceived(channel, control, value, 1);
}

void onSerialNoteOn(byte channel, byte note, byte vel) {
  onExternalMessageReceived(channel, note, vel, 0);
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
    if (channel == external_MIDI_channel[6 + i] && control == external_MIDI_control[6 + i]) {
      sendUSBControlChange(a[i].control[current_layout], value, a[i].channel[current_layout]);
    }
  }
  if (!is_Control) {
    if (type == 0) sendSerialNote(control, value, channel);
    if (type == 1) sendSerialControlChange(control, value, channel);
   // if (type == 2) sendSerialPC(control, channel);
  }
}
