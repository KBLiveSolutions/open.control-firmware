

Adafruit_USBD_WebUSB usb_web;
long buffer_delay = 200;
bool buffer_empty = HIGH;
long buffer_time = 0;
byte iii = 0;

void  send_params(byte _key, byte layout_number, byte _num, byte _cc, byte _ch, byte _type) {
  byte buf[6] = {_key, layout_number,  _num,  _cc,  _ch, _type};
    buffer_empty = LOW;
    usb_web.write(buf, 6);
  pixels.setPixelColor(1, 0, 0, 100);
  pixels.show();
}


void  test() {

  // for (byte layout_number = 0; layout_number < NUM_LAYOUT; layout_number++) {
//  for (byte i = 0; i < 16; i ++)
if (iii < 16){
    buffer_time = millis();
    byte layout_number = 0;
    byte _num = iii;
    byte _key = 10;
    byte _cc = b[0].short_control[layout_number];
    byte _ch = b[0].short_ch[layout_number];
    byte _type = b[0].short_type[layout_number];
    send_params(_key, layout_number, _num, _cc, _ch, _type);
  iii ++;
  }  
/*  for (byte i = 0; i < NUM_BUTTONS; i ++) {
    byte layout_number = 0;
    byte _num = i;
     byte _key = 12;
     byte _cc = b[i].long_control[layout_number];
     byte _ch = b[i].long_ch[layout_number];
     byte _type = b[i].long_type[layout_number];
  //   send_params(_key, layout_number, _num, _cc, _ch, _type);
  }*/
  //}
}

void line_state_callback(bool connected)
{
  if ( connected ) {
    //   usb_web.println("Connected" );
    test() ;
    pixels.setPixelColor(1, 0, 100, 0);
    pixels.show();
  }
}

// the setup function runs once when you press reset or power the board
void setup_webusb()
{
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  pixels.setPixelColor(1, 100, 0, 0);
  pixels.show();


  // usb_web.setLandingPage(&landingPage);
  // usb_web.setLineStateCallback(line_state_callback);
  //usb_web.setStringDescriptor("TinyUSB WebUSB");
  usb_web.begin();

  // wait until device mounted
  while ( !TinyUSBDevice.mounted() ) delay(1);

}


void usbweb_loop()
{
  if ( (millis()-buffer_time) > buffer_delay){
  test();
   // buffer_empty = HIGH; 
  }  
  if (usb_web.available() < 7) return;

  uint8_t input[7];
  usb_web.readBytes(input, 7);
}
