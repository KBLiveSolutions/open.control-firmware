
void line_state_callback(bool connected)
{
  // connected = green, disconnected = red
  //  send_data(connected ? HIGH : LOW);
  pixels.fill(connected ? 0x00ff00 : 0xff0000);
  pixels.show();
}

void setup_webusb()
{
  usb_web.setLineStateCallback(line_state_callback);
  usb_web.begin();
  while ( !TinyUSBDevice.mounted() ) delay(1);
}

bool sysex_in = LOW;

const int BUFFER_SIZE = 20;
char sysex_buffer[BUFFER_SIZE];

void usbweb_loop()
{
  if (usb_web.available() > 0) {

    byte inByte = usb_web.read();

    // sendUSBControlChange(inByte, int(inByte/2), 1);
    if (inByte == 140 && !sysex_in)  {
      global_dump(HIGH);
    }
    if (inByte == 240) {
      sysex_in = HIGH;
    }
    if (sysex_in) {
      int len = usb_web.readBytesUntil(247, sysex_buffer, BUFFER_SIZE);
      byte sysex_data[len + 2];
      sysex_data[0] = 240;
      for (byte i = 0; i < len+1; i++) {
        sysex_data[i + 1] = sysex_buffer[i];
      }
      sysex_data[len + 1] = 247;
      USB_MIDI.sendSysEx(len+2, sysex_data, true);
      onSysEx(sysex_data, len+2, LOW);
      sysex_in = LOW;
    }

  }
}
