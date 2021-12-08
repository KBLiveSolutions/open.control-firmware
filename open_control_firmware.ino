
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "RPi_Pico_TimerInterrupt.h"
#include <EEPROM.h>
#include <MIDI.h>
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, USB_MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, SERIAL_MIDI);

#define User_Input_TIMER_INTERVAL 300
#define NUM_BUTTONS 8
#define NUM_LEDS 6
#define NUM_SLIDERS 2
#define NUM_ENCODERS 2
#define NUM_DISPLAY 1
#define NUM_LAYOUT 3
#define NUM_OPTIONS 10
byte current_layout = 0;
unsigned long scrolling_speed = 200;
long unsigned _now = millis();
int i = 0;
byte options[NUM_OPTIONS];
byte _clock = 0;

#include "LEDS.h"
#include "DISPLAY.h"
#include "MAIN_MIDI.h"
#include "BUTTONS.h"
#include "ANALOG.h"
#include "ROTARY.h"
#include "USB_MIDI.h"
#include "SERIAL_MIDI.h"
// RPI_PICO_Timer User_Input_Timer(3);


void setup_MIDI() {
  //   // these two string must be exactly 32 chars long
  //   //                                   01234567890123456789012345678912
  USBDevice.setManufacturerDescriptor("KB Live Solutions");
  USBDevice.setProductDescriptor("open·control");

  USB_MIDI.begin(MIDI_CHANNEL_OMNI);
  USB_MIDI.turnThruOff();
  USB_MIDI.setHandleControlChange(onUSBControlChange);
  USB_MIDI.setHandleSystemExclusive(onUSBSysEx);
  USB_MIDI.setHandleNoteOn(onUSBNoteOn);
  USB_MIDI.setHandleNoteOff(onUSBNoteOff);
  USB_MIDI.setHandleClock(clock_received);
  USB_MIDI.setHandleStop(clock_stop);
  USB_MIDI.setHandleStart(clock_start);
  USB_MIDI.setHandleContinue(clock_continue);

  SERIAL_MIDI.begin(MIDI_CHANNEL_OMNI);
  SERIAL_MIDI.turnThruOff();
  SERIAL_MIDI.setHandleControlChange(onSerialControlChange);
  SERIAL_MIDI.setHandleSystemExclusive(onSerialSysEx);
  SERIAL_MIDI.setHandleNoteOn(onSerialNoteOn);
  SERIAL_MIDI.setHandleNoteOff(onSerialNoteOn);
  SERIAL_MIDI.setHandleClock(clock_received);
  SERIAL_MIDI.setHandleStop(clock_stop);
  SERIAL_MIDI.setHandleStart(clock_start);
}

void setup_display() {
  pinMode(latch_Col, OUTPUT);
  pinMode(clock_Col, OUTPUT);
  pinMode(data_Col, OUTPUT);
  pinMode(toggle_Col, OUTPUT);
  for (int i = 0; i < 6; i++) {
    pinMode(row_pins[i], OUTPUT);
  }
  int init_text[5] = { 79, 80, 105, 67, 79 };
  disp.build_text(5, init_text);
}

void setup_EEPROM() {
  EEPROM.begin(512);
  //   byte default_short[3][NUM_BUTTONS] = {{13, 1, 2, 14, 15, 50, 56, 57}, {22, 24, 26, 18, 19, 50, 56, 57}, {1, 6, 7, 9, 8, 50, 56, 57}};
  //   byte default_long [3][NUM_BUTTONS] = {{0, 17, 3, 0, 0, 51, 56, 57}, {0, 0, 0, 0, 0, 51, 56, 57}, {0, 0, 0, 0, 0, 51, 56, 57}};
  //   int default_leds [3][6] = {{13, 1, 2, 14, 15, 50}, {22, 24, 26, 18, 19, 50}, {1, 6, 7, 9, 8, 50}};
  //   byte default_sliders [3][2] = {{73, 59}, {61, 62}, {61, 62}};
  //   byte default_rotary [3][2] = {{78, 79}, {78, 79}, {78, 79}};
  //   byte default_display [3] = {0, 1, 4};
  byte _byte;
  for (byte layout_num = 0; layout_num < NUM_LAYOUT; layout_num++) {
    for (byte i = 0; i < NUM_BUTTONS; i++) {
      _byte = EEPROM.read(layout_num * 100 + i);
      if (_byte != 255) b[i].short_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 8 + i);
      if (_byte != 255) b[i].short_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 16 + i);
      if (_byte != 255) b[i].short_ch[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 24 + i);
      if (_byte != 255) b[i].short_toggle[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 32 + i);
      if (_byte != 255) b[i].snap[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 40 + i);
      if (_byte != 255) b[i].long_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 48 + i);
      if (_byte != 255) b[i].long_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 56 + i);
      if (_byte != 255) b[i].long_ch[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 64 + i);
      if (_byte != 255) b[i].long_toggle[layout_num] = _byte;
    }

    for (byte i = 0; i < NUM_LEDS; i++) {
      // l[i].led_control[layout_num] = default_leds[layout_num][i];
      _byte = EEPROM.read(layout_num * 100 + 72 + i);
      if (_byte != 255) l[i].led_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 78 + i);
      if (_byte != 255) l[i].led_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 84 + i);
      if (_byte != 255) l[i].led_channel[layout_num] = _byte;
    }

    for (byte i = 0; i < NUM_ENCODERS; i++) {
      _byte = EEPROM.read(layout_num * 100 + 90 + i);
      if (_byte != 255) r[i].control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 92 + i);
      if (_byte != 255) r[i].control_hold[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 94 + i);
      if (_byte != 255) r[i].channel[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 96 + i);
      if (_byte != 255) r[i].channel_hold[layout_num] = _byte;
    }

    for (byte i = 0; i < NUM_SLIDERS; i++) {
      _byte = EEPROM.read(layout_num * 100 + 98 + i);
      if (_byte != 255) a[i].control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 100 + i);
      if (_byte != 255) a[i].channel[layout_num] = _byte;
    }
    _byte = EEPROM.read(layout_num * 100 + 102);
    if (_byte != 255) disp.layout[layout_num] = _byte;
  }


  for (byte i = 0; i < 2; i++) {
    // Retrieve Options and send them to Live
    _byte = EEPROM.read(350 + i);
    if (_byte != 255) {
      byte sysex_array[9] = { 240, 122, 29, 1, 19, 30, i, _byte, 247 };
      sendUSBSysEx(sysex_array, 9);
      options[i] = _byte;
    }
  }

  for (byte i = 0; i < 10; i++) {
    // Retrieve External MIDI
    _byte = EEPROM.read(310 + i);
    if (_byte != 255) external_MIDI_type[i] = _byte;
    _byte = EEPROM.read(320 + i);
    if (_byte != 255) external_MIDI_control[i] = _byte;
    _byte = EEPROM.read(330 + i);
    if (_byte != 255) external_MIDI_channel[i] = _byte;
  }

  _byte = EEPROM.read(340);
  if (_byte != 255) matrix_brightness = _byte;  // Retrieve Display Brightness
  _byte = EEPROM.read(341);
  if (_byte != 255) BRIGHTNESS = _byte;  // Retrieve LED Brightness
  pixels.setBrightness(BRIGHTNESS);
  _byte = EEPROM.read(342);
  if (_byte != 255) USB_thru = _byte;  // Retrieve USB_thru
  _byte = EEPROM.read(343);
  if (_byte != 255) SERIAL_thru = _byte;  // Retrieve SERIAL_thru
}

void clear_EEPROM() {
  EEPROM.begin(512);
  for (byte i = 0; i < 350; i++) {
    EEPROM.write(i, 255);
    EEPROM.commit();
  }
}

/*
bool Check_User_Input(struct repeating_timer *t) {
  b[i].update_button();
  // a[i % 2].check_pot();
  r[i % 2].update_rotary();
  i++;
  if (i == NUM_BUTTONS) i = 0;
  if (millis() - _now > scrolling_speed / 2) {
    disp.inc_scroll();
    _now = millis();
  }
  return true;
}*/

void Check_User_Input() {
  b[i].update_button();
  // a[i % 2].check_pot();
  r[i % 2].update_rotary();
  i++;
  if (i == NUM_BUTTONS) i = 0;
}
void setup() {
  // clear_EEPROM();
  setup_EEPROM();
  init_LEDS();
  setup_MIDI();
  setup_display();
 // User_Input_Timer.attachInterruptInterval(User_Input_TIMER_INTERVAL, Check_User_Input);
  Show_Page_Timer.attachInterruptInterval(Show_Page_TIMER_INTERVAL, Show_Page_Timer_Handler);
}

void setup1(){
 // Display_Timer.attachInterruptInterval(Display_TIMER_INTERVAL, Display_Timer_Handler);
}

void loop() {
  USB_MIDI.read();
  SERIAL_MIDI.read();
  Check_User_Input();
}

long unsigned _now_micro = micros();
void loop1(){
  if ( (micros()-_now_micro) > 27){
  Display_Handler();
    _now_micro =  micros();
  }
  if (millis() - _now > scrolling_speed / 2) {
    disp.inc_scroll();
    _now = millis();
  }
  // Display_Handler();
 // delayMicroseconds(13);
}
