void check_led(byte channel, byte control, byte value, bool serial) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (l[i].led_control[current_layout] == control) {
      l[i].set_color(value, channel);
      l[i].led_update(b[i].btn_state);
    }
    if (!serial) delay(1);
  }
}

void check_rotary(byte channel, byte control, byte value) {
  for (int i = 0; i < NUM_ENCODERS; i++) {
    if (r[i].control[current_layout] == control && !r[i].enc_state) r[i]._value = value;
  }
}

void check_slider(byte channel, byte control, byte value) {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    if (a[i].control[current_layout] == control) a[i]._value = value;
  }
}

void check_custom_led() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (l[i].led_control[current_layout] > 128) {
      l[i].set_color(l[i].led_control[current_layout] - 128, 16);
      l[i].led_update(b[i].btn_state);
    }
  }
}

// =============   MIDI RECEIVE =================


void onUSBControlChange(byte channel, byte control, byte value) {
  check_led(channel, control, value, LOW);
  check_rotary(channel, control, value);
}

void onUSBNoteOn(byte channel, byte note, byte vel) {
  SERIAL_MIDI.sendNoteOn(note, vel, channel);
}

void onUSBNoteOff(byte channel, byte note, byte vel) {
  SERIAL_MIDI.sendNoteOff(note, vel, channel);
}
// =============   MIDI SEND =================

void sendUSBNote(byte control, byte value, byte channel) {
  USB_MIDI.sendNoteOn(control, value, channel);
}

void sendPC(byte program, byte channel) {
}

void sendUSBControlChange(byte control, byte value, byte channel) {
  USB_MIDI.sendControlChange(control, value, channel);
}

void sendUSBSysEx(const uint8_t *data, int _size) {
  USB_MIDI.sendSysEx(_size, data, true);
}


void onUSBSysEx(uint8_t *data, unsigned int _length) {
  if (data[1] == 122 && data[2] == 29 && data[3] == 1 && data[4] == 19) {
    switch (data[5]) {
        // Connect Disconnect

      case 1:
        {                                                                  // Handshake with Editor
          byte sysexArrayBoot[] = { 240, 122, 29, 1, 19, 68, FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION, 247 };  //String that answers to the MIDI Remote Script for Ableton Live
          sendUSBSysEx(sysexArrayBoot, 9);
        }
        break;

      case 2:
        {                                                           // Handshake with Live
          byte sysexArrayBoot[] = { 240, 122, 29, 1, 19, 2, 247 };  //String that answers to the MIDI Remote Script for Ableton Live
          sendUSBSysEx(sysexArrayBoot, 7);
          for (byte i = 0; i < 10; i++) {  // sending the options
            byte option_value = EEPROM.read(300 + i);
            if (option_value != 255) {
              byte _option = 30 + i;
              byte sysex_array[8] = { 240, 122, 29, 1, 19, _option, option_value, 247 };
              sendUSBSysEx(sysex_array, 8);
            }
          }
        }
        break;

      case 3: {   // Disconnect message received
          int disconnect_text[5] = { 13, 13, 13, 13, 13};
          disp.build_text(5, disconnect_text);
          init_LEDS();
        }
        break;


        // Configuration From Editor

      case 4:
        {  // Dump: Receiving {240, 122, 29, 1, 19, 4} from the Editor send each control 1 by 1 {240, 122, 29, 1, 19, 77, Layout, Control, CC Number, Channel, Type, 247}
          byte sysex_to_send[12] = { 240, 122, 29, 1, 19, 4, 0, 0, 0, 0, 0, 247 };
        //  byte sysex_to_send_options[9] = { 240, 122, 29, 1, 19, 17, 0, 0, 247 };
          for (byte layout_number = 0; layout_number < NUM_LAYOUT; layout_number++) {
            sysex_to_send[6] = layout_number;
            // sysex_to_send_snap[6] = layout_number;
            for (byte i = 0; i < NUM_BUTTONS; i += 1) {
              // Retrieve and send short button values
              sysex_to_send[5] = 10;
              sysex_to_send[7] = i;
              sysex_to_send[8] = b[i].short_control[layout_number];
              sysex_to_send[9] = b[i].short_ch[layout_number];
              sysex_to_send[10] = b[i].short_type[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
              // Retrieve and send snap values
              sysex_to_send[5] = 16;
              sysex_to_send[7] = i;
              sysex_to_send[8] = b[i].snap[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
              // Retrieve and send long button values
              sysex_to_send[5] = 11;
              sysex_to_send[7] = i;
              sysex_to_send[8] = b[i].long_control[layout_number];
              sysex_to_send[9] = b[i].long_ch[layout_number];
              sysex_to_send[10] = b[i].long_type[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
              // Retrieve and send double button values
              sysex_to_send[5] = 19;
              sysex_to_send[7] = i;
              sysex_to_send[8] = b[i].double_control[layout_number];
              sysex_to_send[9] = b[i].double_ch[layout_number];
              sysex_to_send[10] = b[i].double_type[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
              // Retrieve and send LEDs values
            }
            for (byte i = 0; i < NUM_LEDS; i += 1) {
              sysex_to_send[5] = 12;
              sysex_to_send[7] = i;
              byte ctrl = l[i].led_control[layout_number];
              if (ctrl > 128) {
                sysex_to_send[8] = ctrl - 128;
                sysex_to_send[9] = 10;
              } else {
                sysex_to_send[8] = ctrl;
                sysex_to_send[9] = l[i].led_channel[layout_number];
              }
              sysex_to_send[10] = 1;
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
            }
            for (byte i = 0; i < NUM_SLIDERS; i += 1) {
              // Retrieve and send sliders values
              sysex_to_send[5] = 13;
              sysex_to_send[7] = i;
              sysex_to_send[8] = a[i].control[layout_number];
              sysex_to_send[9] = a[i].channel[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
            }
            for (byte i = 0; i < NUM_ENCODERS; i += 1) {
              // Retrieve and send sliders values
              sysex_to_send[5] = 14;
              sysex_to_send[7] = i;
              sysex_to_send[8] = r[i].control[layout_number];
              sysex_to_send[9] = r[i].channel[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
              // Retrieve and send sliders values
              sysex_to_send[5] = 14;
              sysex_to_send[7] = i+2;
              sysex_to_send[8] = r[i].control_hold[layout_number];
              sysex_to_send[9] = r[i].channel_hold[layout_number];
              sendUSBSysEx(sysex_to_send, 12);
              delay(2);
            }

            // Retrieve and send display values
            sysex_to_send[5] = 15;
            sysex_to_send[8] = disp.layout[layout_number];
            sysex_to_send[9] = 1;
            sysex_to_send[10] = 1;
            sendUSBSysEx(sysex_to_send, 12);
            delay(2);
          }

          // Retrieve and send External MIDI values
          for (byte i = 0; i < 10; i += 1) {
            sysex_to_send[5] = 18;
            sysex_to_send[6] = 0;
            sysex_to_send[7] = i;
            sysex_to_send[8] = external_MIDI_control[i];
            sysex_to_send[9] = external_MIDI_channel[i];
            sysex_to_send[10] = external_MIDI_type[i];
            sendUSBSysEx(sysex_to_send, 12);
            delay(2);
          }

          // Retrieve and send Options
          for (byte i = 0; i < 2; i++) {
            sysex_to_send[5] = 17;
            sysex_to_send[6] = i;
            sysex_to_send[7] = options[i];
            sendUSBSysEx(sysex_to_send, 12);
            delay(2);
          }

          byte end_array[7] = { 240, 122, 29, 1, 19, 79, 247 };
          sendUSBSysEx(end_array, 7);
        }
        break;

      case 10:
        {  // Set Short Buttons Type Control and Channel
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte btn_ctrl = data[8];
          byte btn_type = data[9];
          byte btn_chnl = data[10];
          b[num].short_type[rcvd_layout] = btn_type;
          b[num].short_control[rcvd_layout] = btn_ctrl;
          b[num].short_ch[rcvd_layout] = btn_chnl;

          eeprom_store(rcvd_layout, num, btn_type);
          eeprom_store(rcvd_layout, num + 8, btn_ctrl);
          eeprom_store(rcvd_layout, num + 16, btn_chnl);
          if (btn_type == 1) {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;


      case 11:
        {  // Sets Button Snap Value
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte snap = data[8];
          b[num].snap[rcvd_layout] = snap;
          eeprom_store(rcvd_layout, num + 32, snap);
        }
        break;


      case 12:
        {  // Set Long Buttons Type Control and Channel
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte btn_ctrl = data[8];
          byte btn_type = data[9];
          byte btn_chnl = data[10];
          b[num].long_type[rcvd_layout] = btn_type;
          b[num].long_control[rcvd_layout] = btn_ctrl;
          b[num].long_ch[rcvd_layout] = btn_chnl;
          eeprom_store(rcvd_layout, num + 40, btn_type);
          eeprom_store(rcvd_layout, num + 48, btn_ctrl);
          eeprom_store(rcvd_layout, num + 56, btn_chnl);
          if (btn_type == 1) {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;

      case 19:
        {  // Set Double Press Buttons Type Control and Channel
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte btn_ctrl = data[8];
          byte btn_type = data[9];
          byte btn_chnl = data[10];
          b[num].double_type[rcvd_layout] = btn_type;
          b[num].double_control[rcvd_layout] = btn_ctrl;
          b[num].double_ch[rcvd_layout] = btn_chnl;
          eeprom_store(rcvd_layout, num + 364, btn_type);
          eeprom_store(rcvd_layout, num + 372, btn_ctrl);
          eeprom_store(rcvd_layout, num + 380, btn_chnl);
          if (btn_type == 1) {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;


      case 13:
        {  // Sets Button Toggle Value
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte toggle = data[8];
          if (data[9] == 0) {
            b[num].short_toggle[rcvd_layout] = toggle;
            eeprom_store(rcvd_layout, num + 24, toggle);
          } else {
            b[num].long_toggle[rcvd_layout] = toggle;
            eeprom_store(rcvd_layout, num + 64, toggle);
          }
        }
        break;

      case 14:
        {  // Sets LEDs Control and Channel
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte led_ctrl = data[8];
          byte led_type = data[9];
          byte led_chnl = data[10];
          l[num].led_control[rcvd_layout] = led_ctrl;
          l[num].led_channel[rcvd_layout] = led_chnl;
          eeprom_store(rcvd_layout, num + 72, led_type);
          eeprom_store(rcvd_layout, num + 78, led_ctrl);
          eeprom_store(rcvd_layout, num + 84, led_chnl);
          {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;

      case 15:
        {  // Sets LEDs Control and Channel for Custom Color
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte led_ctrl = data[8] + 128;  // led_ctrl is stored > 128 to differentiate from "standard" LED colors
          byte led_chnl = data[10];
          l[num].led_control[rcvd_layout] = led_ctrl;
          l[num].led_channel[rcvd_layout] = led_chnl;
          eeprom_store(rcvd_layout, 78, led_ctrl);
          eeprom_store(rcvd_layout, 84, led_chnl);
          check_custom_led();
        }
        break;

      case 16:
        {  // Set Rotary Encoder Control and Channel
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte control = data[8];
          byte channel = data[10];
          if (num < 2){
          r[num].control[rcvd_layout] = control;
          r[num].channel[rcvd_layout] = channel;
          }
          else{
          r[num-2].control_hold[rcvd_layout] = control;
          r[num-2].channel_hold[rcvd_layout] = channel;
          }
          eeprom_store(rcvd_layout, num + 90, control);
          eeprom_store(rcvd_layout, num + 94, channel);
          {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;

      case 17:
        {  // Sets Sliders Control and Channel
          byte rcvd_layout = data[6];
          byte num = data[7];
          byte control = data[8];
          byte channel = data[10];
          a[num].control[rcvd_layout] = control;
          a[num].channel[rcvd_layout] = channel;
          eeprom_store(rcvd_layout, num + 98, control);
          eeprom_store(rcvd_layout, num + 100, channel);
          {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;

      case 18:
        {  // Sets Display Controls
          byte rcvd_layout = data[6];
          byte layout = data[8];
          disp.layout[rcvd_layout] = layout;
          eeprom_store(rcvd_layout, 102, layout);
          {
            byte acknowledgment_array[7] = { 240, 122, 29, 1, 19, 78, 247 };
            sendUSBSysEx(acknowledgment_array, 7);
          }
        }
        break;

      case 21:
        {  // Sets External MIDI Type Control and Channel

          byte datatype = data[6];
          byte but_num = data[7];
          byte value = data[8];
          if (datatype == 0) {
            external_MIDI_type[but_num] = value;
            raw_eeprom_store(310 + but_num, value);
          } 
          else if (datatype == 1) {
            external_MIDI_control[but_num] = value;
            raw_eeprom_store(320 + but_num, value);
          } 
          else if (datatype == 2) {
            external_MIDI_channel[but_num] = value;
            raw_eeprom_store(330 + but_num, value);
          }
        }
        break;

      case 22:
        {  // Sets Display Brightness
          matrix_brightness = (127 - data[6]) * 2;
          raw_eeprom_store(340, matrix_brightness);
        }
        break;

      case 23:
        {  // Sets LEDs Brightness
          BRIGHTNESS = data[6] * 2;
          pixels.setBrightness(BRIGHTNESS);
          pixels.show();
          raw_eeprom_store(341, BRIGHTNESS);
        }
        break;

      case 25:
        {  // Sets USB Through
          USB_thru = data[6];
          raw_eeprom_store(342, USB_thru);
        }
        break;

      case 26:
        {  // Sets SERIAL Through
          SERIAL_thru = data[6];
          raw_eeprom_store(343, SERIAL_thru);
        }
        break;

      case 27:
        {  // Sets Pedal Min and Max
          int pedal_number = data[7];
          int min_or_max = data[8];
          a[pedal_number]._calibrate(pedal_number, min_or_max);
        }
        break;

      case 30:
        {  // Options
          options[data[6]] =  data[7];
          raw_eeprom_store(350 + data[6], data[7]);
          byte data_array[9] = { 240, 122, 29, 1, 19, 30, data[6], data[7], 247};
          sendUSBSysEx(data_array, 9);
         // sendSerialSysEx(data_array, 9);
        }
        break;


        // receive Data from Live

      case 40:
        {  // Layout Value received
          clear_leds();
          disp.clear_text();
          current_layout = data[6];
          int page_text[MAX_CHAR] = { 48, 65, 71, 69, 0, (current_layout+17) };
          disp.build_text(6, page_text);
          showing_page = HIGH;
         _now_page = millis();
          check_custom_led();
          for (int i=0 ; i<NUM_SLIDERS ; i++){
            a[i].slider_state = LOW;
          }
        }
        break;

      case 51:
        {  // Text received
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
        {  // Direct Text received
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
        {  // Receive Looper number from Live
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
        {  // Receive Looper Clear All
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
