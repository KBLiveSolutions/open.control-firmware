

Adafruit_USBD_WebUSB usb_web;
long buffer_delay = 200;
bool buffer_empty = HIGH;
long buffer_time = 0;
byte iii = 0;
/*
  void  send_params(byte _key, byte layout_number, byte _num, byte _cc, byte _ch, byte _type) {
  //  byte buf[8] = {240, 122, 29, 1 20, _key, layout_number,  _num,  _cc,  _ch, _type, 247};
  //  usb_web.write(buf, 8);
  }*/

void sendWebUSB(byte *buf, byte _length) {
  usb_web.write(buf, _length);
}

void line_state_callback(bool connected)
{
  // connected = green, disconnected = red
  //  send_data(connected ? HIGH : LOW);
  pixels.fill(connected ? 0x00ff00 : 0xff0000);
  pixels.show();
}
/*
  void  send_data() {

  // for (byte layout_number = 0; layout_number < NUM_LAYOUT; layout_number++) {
  for (byte i = 0; i < 6; i ++) {
    byte layout_number = 0;
    byte _num = i;
    byte _key = 10;
    byte _cc = b[i].short_control[layout_number];
    byte _ch = b[i].short_ch[layout_number];
    byte _type = b[i].short_type[layout_number];
    send_params(_key, layout_number, _num, _cc, _ch, _type);
  }
  }*/

void global_dump(bool usbweb)  { // Dump: Receiving {240, 122, 29, 1, 19, 4} from the Editor send each control 1 by 1 {240, 122, 29, 1, 19, 77, Layout, Control, CC Number, Channel, Type, 247}
  byte sysex_to_send[12] = { 240, 122, 29, 1, 20, 4, 0, 0, 0, 0, 0, 247 };
  for (byte layout_number = 0; layout_number < NUM_LAYOUT; layout_number++) {
    sysex_to_send[6] = layout_number;
    for (byte i = 0; i < NUM_BUTTONS; i += 1) {
      sysex_to_send[7] = i;
      // Retrieve and send short button values
      sysex_to_send[5] = 10;
      sysex_to_send[8] = b[i].short_control[layout_number];
      sysex_to_send[9] = b[i].short_ch[layout_number];
      sysex_to_send[10] = b[i].short_type[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      // Retrieve and send snap values
      sysex_to_send[5] = 11;
      sysex_to_send[8] = b[i].snap[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      // Retrieve and send long button values
      sysex_to_send[5] = 12;
      sysex_to_send[8] = b[i].long_control[layout_number];
      sysex_to_send[9] = b[i].long_ch[layout_number];
      sysex_to_send[10] = b[i].long_type[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      // Retrieve and send double button values
      sysex_to_send[5] = 19;
      sysex_to_send[8] = b[i].double_control[layout_number];
      sysex_to_send[9] = b[i].double_ch[layout_number];
      sysex_to_send[10] = b[i].double_type[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      sysex_to_send[5] = 13;
      sysex_to_send[8] = b[i].short_toggle[layout_number];
      sysex_to_send[9] = 0;
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      sysex_to_send[5] = 13;
      sysex_to_send[8] = b[i].long_toggle[layout_number];
      sysex_to_send[9] = 1;
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      sysex_to_send[5] = 13;
      sysex_to_send[8] = b[i].double_toggle[layout_number];
      sysex_to_send[9] = 2;
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      // Retrieve and send LEDs values
    }
    delay(2);
    for (byte i = 0; i < NUM_LEDS; i += 1) {
      sysex_to_send[5] = 14;
      sysex_to_send[7] = i;
      byte ctrl = l[i].led_control[layout_number];
      if (ctrl > 128) {
        sysex_to_send[8] = ctrl - 128;
        sysex_to_send[9] = 10;
      }
      else {
        sysex_to_send[8] = ctrl;
        sysex_to_send[9] = l[i].led_channel[layout_number];
      }
      sysex_to_send[10] = 1;
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
    }
    for (byte i = 0; i < NUM_SLIDERS; i += 1) {
      // Retrieve and send sliders values
      sysex_to_send[5] = 17;
      sysex_to_send[7] = i;
      sysex_to_send[8] = a[i].control[layout_number];
      sysex_to_send[9] = a[i].channel[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
    }
    for (byte i = 0; i < NUM_ENCODERS; i += 1) {
      // Retrieve and send encoders values
      sysex_to_send[5] = 8;
      sysex_to_send[7] = i;
      sysex_to_send[8] = r[i].control[layout_number];
      sysex_to_send[9] = r[i].channel[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
      // Retrieve and send encoders hold values
      sysex_to_send[5] = 9;
      sysex_to_send[7] = i;
      sysex_to_send[8] = r[i].control_hold[layout_number];
      sysex_to_send[9] = r[i].channel_hold[layout_number];
      if (usbweb) sendWebUSB(sysex_to_send, 12);
      else sendUSBSysEx(sysex_to_send, 12);
      delay(2);
    }

    // Retrieve and send display values
    sysex_to_send[5] = 18;
    sysex_to_send[7] = 0;
    sysex_to_send[8] = disp.display_control[layout_number];
    sysex_to_send[9] = 1;
    sysex_to_send[10] = 1;
    if (usbweb) sendWebUSB(sysex_to_send, 12);
    else sendUSBSysEx(sysex_to_send, 12);
    delay(2);

    sysex_to_send[5] = 24;
    sysex_to_send[7] = 0;
    sysex_to_send[8] = linked_page[layout_number];
    sysex_to_send[9] = 1;
    sysex_to_send[10] = 1;
    if (usbweb) sendWebUSB(sysex_to_send, 12);
    else sendUSBSysEx(sysex_to_send, 12);
    delay(2);
  }

  // Retrieve and send External MIDI values
  for (byte i = 0; i < 10; i += 1) {
    sysex_to_send[5] = 21;
    sysex_to_send[6] = 0;
    sysex_to_send[7] = i;
    sysex_to_send[8] = external_MIDI_control[i];
    sysex_to_send[9] = external_MIDI_channel[i];
    sysex_to_send[10] = external_MIDI_type[i];
    if (usbweb) sendWebUSB(sysex_to_send, 12);
    else sendUSBSysEx(sysex_to_send, 12);
    delay(2);
  }

  // Retrieve and send Options
  for (byte i = 0; i < 2; i++) {
    sysex_to_send[5] = 30;
    sysex_to_send[6] = i;
    sysex_to_send[7] = options[i];
    if (usbweb) sendWebUSB(sysex_to_send, 12);
    else sendUSBSysEx(sysex_to_send, 12);
    delay(2);
  }

  byte end_array[7] = { 240, 122, 29, 1, 19, 79, 247 };
  if (usbweb)sendWebUSB(end_array, 7);
  else sendUSBSysEx(end_array, 7);
}

void setup_webusb()
{
  usb_web.setLineStateCallback(line_state_callback);
  usb_web.begin();
  while ( !TinyUSBDevice.mounted() ) delay(1);
  byte sysex_test[5] = {240, 129, 23, 45, 247};
  //   inputToBytes(input, 12);
  sendUSBSysEx(sysex_test, 5);
}

const int BUFFER_SIZE = 12;
byte _buffer[BUFFER_SIZE];
byte inputBytes[BUFFER_SIZE];
void inputToBytes(uint8_t *input, byte _l) {
  for (byte i = 0; i < _l; i++) {
    inputBytes[i] = int(input);
  }
}
bool sysex_in = LOW;
byte sysex_data[12];

void buildSysex(byte inByte) {

}
void usbweb_loop()
{
  if (usb_web.available() > 0) {
  // uint8_t input[12];
  // usb_web.readBytes(input, 12);
  
  byte inByte = usb_web.read();

  // sendUSBControlChange(inByte, int(inByte/2), 1);
  if (inByte == 140 && !sysex_in)  {
    byte Live_update_request[7] = { 240, 122, 29, 1, 19, 101, 247 };
    sendUSBSysEx(Live_update_request, 7);
    global_dump(HIGH);
  }
  if (inByte == 240) {
    sysex_in = HIGH;
  }
  if (sysex_in) {
    usb_web.readBytesUntil(247, sysex_data, 12);
     for (byte i=10; i>0; i--){
      sysex_data[i+1] = sysex_data[i];
     }
     sysex_data[0] = 240;
     sysex_data[1] = 122;
     sysex_data[11] = 247;
     USB_MIDI.sendSysEx(12, sysex_data, true);
    onSysEx(sysex_data, 12, LOW);
    sysex_in = LOW;
  }
    
  }
}
