

WEBUSB_URL_DEF(landingPage, 1 /*https*/, "adafruit.github.io/Adafruit_TinyUSB_Arduino/examples/webusb-rgb/index.html");

void open_web_editor(){
  usb_web.setLandingPage(&landingPage);
//  usb_web.begin();
}


void check_editor() {
  if(b[6].btn_state && b[7].btn_state) open_web_editor();
}

void line_state_callback(bool connected)
{
  
  byte editor_handshake[] = { 240, 122, 29, 1, 19, 68, FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION, 247 };  //String that answers to the MIDI Remote Script for Ableton Live
  sendWebUSB(editor_handshake, 9);
  // connected = green, disconnected = red
  //  send_data(connected ? HIGH : LOW);
  // pixels.fill(connected ? 0x00ff00 : 0xff0000);
  // pixels.show();
}

void setup_webusb()
{
  // usb_web.setLandingPage(&landingPage);
  usb_web.setLineStateCallback(line_state_callback);
  usb_web.begin();
  while ( !TinyUSBDevice.mounted() ) delay(1);
}

bool sysex_in = LOW;

const int BUFFER_SIZE = 20;
char sysex_buffer[BUFFER_SIZE];

void WebUSB()
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
      onSysEx(sysex_data, len+2, LOW, HIGH);
      sysex_in = LOW;
    }

  }
}
