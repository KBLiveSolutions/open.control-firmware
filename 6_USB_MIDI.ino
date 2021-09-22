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


void onUSBSysEx(const uint8_t *data) { //, unsigned _length, bool complete) {

  if (data[1] == 122 && data[2] == 29 && data[3] == 1 && data[4] == 19) {
    if (data[5] < 30) {
      switch (data[5]) {

        // Connect Disconnect

        case 1: {    // Handshake with Editor
            byte sysexArrayBoot[] = {240, 122, 29, 1, 19, 68, 1, 0, 247};  //String that answers to the MIDI Remote Script for Ableton Live
            sendUSBSysEx(sysexArrayBoot, 9);
          }
          break;

        case 2: {    // Handshake with Live
            byte sysexArrayBoot[] = {240, 122, 29, 1, 19, 2, 247};  //String that answers to the MIDI Remote Script for Ableton Live
            sendUSBSysEx(sysexArrayBoot, 7);
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


        // Configuration From Editor

        case 4: { // Receiving {240, 122, 29, 1, 19, 4} from the Editor send each control 1 by 1 {240, 122, 29, 1, 19, 77, Layout, Control, CC Number, Channel, Type, 247}
            byte sysex_to_send[12] = {240, 122, 29, 1, 19, 4, 0, 0, 0, 0, 0, 247};
            byte sysex_to_send_snap[10] = {240, 122, 29, 1, 19, 16, 0, 0, 0, 247};
            byte sysex_to_send_options[9] = {240, 122, 29, 1, 19, 17, 0, 0, 247};
            for ( byte layout_number = 0; layout_number < NUM_LAYOUT; layout_number ++) {
              sysex_to_send[6] = layout_number;
              sysex_to_send_snap[6] = layout_number;
              for ( byte i = 0; i < NUM_BUTTONS; i += 1) {
                // Retrieve and send short button values
                sysex_to_send[7]  = i;
                sysex_to_send[8]  = b[i].short_control[layout_number];
                sysex_to_send[9] = b[i].short_ch[layout_number];
                sysex_to_send[10] = b[i].short_type[layout_number];
                sendUSBSysEx(sysex_to_send, 12);
                // Retrieve and send snap values
                sysex_to_send_snap[7]  = i;
                sysex_to_send_snap[8]  = b[i].snap[layout_number];
                sendUSBSysEx(sysex_to_send_snap, 10);
                // Retrieve and send long button values
                sysex_to_send[7]  = i + 6;
                sysex_to_send[8]  = b[i].long_control[layout_number];
                sysex_to_send[9] = b[i].long_ch[layout_number];
                sysex_to_send[10] = b[i].long_type[layout_number];
                sendUSBSysEx(sysex_to_send, 12);
                // Retrieve and send LEDs values
                sysex_to_send[7]  = i + 12;
                byte ctrl = l[i].control[layout_number];
                if (ctrl > 128) {
                  sysex_to_send[8]  = ctrl - 128;
                  sysex_to_send[9] = 10;
                }
                else {
                  sysex_to_send[8]  = ctrl;
                  sysex_to_send[9] = l[i].state;
                }
                sysex_to_send[10] = 1;
                sendUSBSysEx(sysex_to_send, 12);
              }
              for ( byte i = 0; i < NUM_SLIDERS; i += 1) {
                // Retrieve and send sliders values
                sysex_to_send[7]  = i + 18;
                sysex_to_send[8]  = a[i].control[layout_number];
                sysex_to_send[9] = a[i].channel[layout_number];
                sendUSBSysEx(sysex_to_send, 12);
              }

              // Retrieve and send display values
              sysex_to_send[7]  = 20;
              sysex_to_send[8]  = disp.layout[layout_number];
              sysex_to_send[9] = 1;
              sysex_to_send[10] = 1;
              sendUSBSysEx(sysex_to_send, 12);
            }

            // Retrieve and send External MIDI values
            for ( byte i = 0; i < NUM_BUTTONS + NUM_SLIDERS; i += 1) {
              sysex_to_send[6] = 0;
              sysex_to_send[7]  = i + 22;
              sysex_to_send[8] = external_MIDI_control[i];
              sysex_to_send[9] = external_MIDI_channel[i];
              sysex_to_send[10]  = external_MIDI_type[i];
              sendUSBSysEx(sysex_to_send, 12);
            }

            // Retrieve and send Options
            for (byte i = 0; i < 10; i++) {
              byte value = EEPROM.read(300 + i);
              sysex_to_send_options[6] = i;
              sysex_to_send_options[7] = value;
              sendUSBSysEx(sysex_to_send_options, 9);
            }

            byte end_array[7] = {240, 122, 29, 1, 19, 79, 247};
            sendUSBSysEx(end_array, 7);
          }
          break;

        case 10: {    // Set Short Buttons Type Control and Channel
            byte rcvd_layout = data[6];
            byte num = data[7];
            byte btn_ctrl = data[8];
            byte btn_type = data[9];
            byte btn_chnl = data[10];
            b[num].short_type[rcvd_layout] = btn_type;
            b[num].short_control[rcvd_layout] = btn_ctrl;
            b[num].short_ch[rcvd_layout] = btn_chnl;

            eeprom_store(rcvd_layout, num, btn_type, btn_ctrl, btn_chnl);
            if (btn_type == 1)
            { byte acknowledgment_array[7] = {240, 122, 29, 1, 19, 78, 247};
              sendUSBSysEx(acknowledgment_array, 7);
            }
          }
          break;

        case 11:  {    // Set Long Buttons Type Control and Channel
            byte rcvd_layout = data[6];
            byte num = data[7];
            byte btn_ctrl = data[8];
            byte btn_type = data[9];
            byte btn_chnl = data[10];
            b[num].long_type[rcvd_layout] = btn_type;
            b[num].long_control[rcvd_layout] = btn_ctrl;
            b[num].long_ch[rcvd_layout] = btn_chnl;
            eeprom_store(rcvd_layout, num + 6, btn_type, btn_ctrl, btn_chnl);
            if (btn_type == 1)
            { byte acknowledgment_array[7] = {240, 122, 29, 1, 19, 78, 247};
              sendUSBSysEx(acknowledgment_array, 7);
            }
          }
          break;

        case 12: {    // Sets LEDs Control and Channel
            byte rcvd_layout = data[6];
            byte num = data[7];
            byte led_ctrl = data[8];
            byte led_chnl = data[10];
            l[num].control[rcvd_layout] = led_ctrl;
            l[num].state = led_chnl;
            eeprom_store(rcvd_layout, num + 12, 1, led_ctrl, led_chnl);
            // if (btn_type==1)
            { byte acknowledgment_array[7] = {240, 122, 29, 1, 19, 78, 247};
              sendUSBSysEx(acknowledgment_array, 7);
            }
          }
          break;

        case 13: {    // Sets Sliders Control and Channel
            byte rcvd_layout = data[6];
            byte num = data[7];
            byte control = data[8];
            byte channel = data[10];
            a[num].control[rcvd_layout] = control;
            a[num].channel[rcvd_layout] = channel;
            eeprom_store(rcvd_layout, num + 18, 1, control, channel);
            { byte acknowledgment_array[7] = {240, 122, 29, 1, 19, 78, 247};
              sendUSBSysEx(acknowledgment_array, 7);
            }
          }
          break;

        case 14: {    // Sets Display Controls
            byte rcvd_layout = data[6];
            byte layout = data[8];
            disp.layout[rcvd_layout] = layout;
            eeprom_store(rcvd_layout, 20, 0, layout, 0);
            { byte acknowledgment_array[7] = {240, 122, 29, 1, 19, 78, 247};
              sendUSBSysEx(acknowledgment_array, 7);
            }
          }
          break;

        case 15: {  // Sets External MIDI Type Control and Channel

            byte datatype = data[6];
            byte but_num =  data[7];
            byte value = data[8];
            if (datatype == 0) {
              external_MIDI_type[but_num] = value;
              EEPROM.write(276 + but_num, value);
            }
            else if (datatype == 1) {
              external_MIDI_control[but_num] = value;
              EEPROM.write(284 + but_num, value);
            }
            else if (datatype == 2) {
              external_MIDI_channel[but_num] = value;
              EEPROM.write(292 + but_num, value);
            }
          }
          break;

        case 16: {    // Sets Button Snap Value
            byte rcvd_layout = data[6];
            byte num = data[7];
            byte snap = data[8];
            b[num].snap[rcvd_layout] = snap;
            eeprom_store_snap(rcvd_layout, num, snap);
          }
          break;

        case 17: {    // Sets LEDs Control and Channel
            byte rcvd_layout = data[6];
            byte num = data[7];
            byte led_ctrl = data[8] + 128;
            byte led_chnl = data[10];
            l[num].control[rcvd_layout] = led_ctrl;
            l[num].state = led_chnl;
            eeprom_store(rcvd_layout, num + 12, 1, led_ctrl, led_chnl);
            // if (btn_type==1)
            check_custom_led();
          }
          break;


        // receive Input Data from Live

        case 20: {   // Layout Value received
            clear_leds();
            current_layout = data[6];
            // check_layout_led();
            check_custom_led();
          }
          break;

        case 21: {   // Text received
            byte text_len = data[7]; //min(MAX_CHAR, data[7]);
            if (data[6] == disp.layout[current_layout]) {
              disp.clear_text();
              for (byte i = 0; i < text_len ; i++) {
                disp.text[i] =  data[8 + i];
              }
              disp.build_text(text_len);
            }
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

    else { // Options
      EEPROM.write(270 + data[5], data[6]);
      byte data_array[8] = {240, 122, 29, 1, 19, data[5], data[6], 247};
      sendUSBSysEx(data_array, 8);
    }
  }
}

// =============   MIDI RECEIVE =================


void onUSBMIDIPacketReceived(byte header, byte byte1, byte byte2, byte byte3) {
  if (header != 0) {
    switch (header & 0x0F) {
      case 0x0F:
        if (byte1 == 248) { // Clock
          _clock += 1;
          if (_clock % 6 == 0) {
            for (byte i = 0; i < NUM_LEDS; i++) {
              l[i].toggle_led(int(_clock/6));
            }
            if (_clock == 24) _clock = 0;
          }
        }

        if (byte1 == 250) { // Start
          _clock = 0;
          for (byte i = 0; i < NUM_LEDS; i++) {
            l[i].show_color();
          }
        }

        if (byte1 == 252) { // Stop
          for (byte i = 0; i < NUM_LEDS; i++) {
            l[i].show_color();
          }
        }


        break;
      case 0x0C:  // Program Change
      case 0x08:  // Note-off
      case 0x09:  // Note-on
      case 0x0B:  // Control Change
        onUSBControlChange(byte1 - 175, byte2, byte3);
        break;
      case 0x04:  // SysEx starts or continues
        if (!sysex_in) {
          sysex_data[0] = byte1;
          sysex_data[1] = byte2;
          sysex_data[2] = byte3;
          sysex_index = 3;
          sysex_in = HIGH;
        }
        else {
          sysex_data[sysex_index] = byte1;
          sysex_data[sysex_index + 1] = byte2;
          sysex_data[sysex_index + 2] = byte3;
          sysex_index += 3;
        }
        // append sysex buffer with 3 bytes
        break;
      case 0x05:  // Single-byte System Common Message or SysEx ends with the following single byte
        if (sysex_in) {
          sysex_data[sysex_index] = byte1;
          onUSBSysEx(sysex_data);
          sysex_index = 0;
          sysex_in = LOW;
        }
        // append sysex buffer with 1 byte
        // process completed sysex buffer
        // init sysex buffer empty
        break;
      case 0x06:  // SysEx ends with the following two bytes
        if (sysex_in) {
          sysex_data[sysex_index] = byte1;
          sysex_data[sysex_index + 1] = byte2;
          onUSBSysEx(sysex_data);
          sysex_index = 0;
          sysex_in = LOW;
        }
        // append sysex buffer with 2 bytes
        // process completed sysex buffer
        // init sysex buffer empty
        break;
      case 0x07:  // SysEx ends with the following three bytes
        if (sysex_in) {
          sysex_data[sysex_index] = byte1;
          sysex_data[sysex_index + 1] = byte2;
          sysex_data[sysex_index + 2] = byte3;
          onUSBSysEx(sysex_data);
          sysex_index = 0;
          sysex_in = LOW;
        }
        // append sysex buffer with 3 bytes
        // process completed sysex buffer
        // init sysex buffer empty
        break;
    }
  }
}
void onUSBControlChange(byte channel, byte control, byte value) {
  check_led(channel, control, value);
}

// =============   MIDI SEND =================

void sendUSBNote (byte control, byte value, byte channel) {
  midiEventPacket_t event = {0x09, 0x90 | channel - 1, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void sendPC (byte program, byte channel) {
}

void sendUSBControlChange(byte control, byte value, byte channel) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel - 1, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void sendUSBSysEx(const uint8_t *data, size_t size) {
  if (data == NULL || size == 0) return;
  size_t midiDataSize = (size + 2) / 3 * 4;
  uint8_t midiData[midiDataSize];
  const uint8_t *d = data;
  uint8_t *p = midiData;
  size_t bytesRemaining = size;

  while (bytesRemaining > 0) {
    switch (bytesRemaining) {
      case 1:
        *p++ = 5;   // SysEx ends with following single byte
        *p++ = *d;
        *p++ = 0;
        *p = 0;
        bytesRemaining = 0;
        break;
      case 2:
        *p++ = 6;   // SysEx ends with following two bytes
        *p++ = *d++;
        *p++ = *d;
        *p = 0;
        bytesRemaining = 0;
        break;
      case 3:
        *p++ = 7;   // SysEx ends with following three bytes
        *p++ = *d++;
        *p++ = *d++;
        *p = *d;
        bytesRemaining = 0;
        break;
      default:
        *p++ = 4;   // SysEx starts or continues
        *p++ = *d++;
        *p++ = *d++;
        *p++ = *d++;
        bytesRemaining -= 3;
        break;
    }
  }
  MidiUSB.write(midiData, midiDataSize);
  MidiUSB.flush();
}
