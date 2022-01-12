/* # Firmware for open·control
# Copyright (C) 2021 Pierre-Antoine GRISON
# more info on open·control on http://opencontrol.me

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.*/

#define FIRMWARE_MAJOR_VERSION 1
#define FIRMWARE_MINOR_VERSION 1

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <OneButton.h>
#include <EEPROM.h>
#include <MIDI.h>
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, USB_MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, SERIAL_MIDI);


#define NUM_BUTTONS 8
#define NUM_LEDS 6
#define NUM_SLIDERS 2
#define NUM_ENCODERS 2
#define NUM_DISPLAY 1
#define NUM_LAYOUT 3
#define NUM_OPTIONS 10


#include "TABLES.h"
#include "LEDS.h"
#include "DISPLAY.h"
#include "VARIOUS.h"
#include "BUTTONS.h"
#include "ANALOG.h"
#include "ROTARY.h"
#include "MIDI.h"

void setup_Buttons(){
  for (int i=0; i<NUM_BUTTONS; i++){
  b[i].attachSimple(simpleClickcallBack);
  b[i].attachLong(longClickcallBack);
  b[i].attachDouble(doubleClickcallBack);
  b[i].setClickTicks(150);
  b[i].setDebounceTicks(20);
  }
}

void setup_MIDI() {
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
  USB_MIDI.setHandleStart(clock_start_serial);
  USB_MIDI.setHandleContinue(clock_continue);
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
  EEPROM.begin(1024);
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
      
      _byte = EEPROM.read(layout_num * 100 + 364 + i);
      if (_byte != 255) b[i].double_type[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 372 + i);
      if (_byte != 255) b[i].double_control[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 380 + i);
      if (_byte != 255) b[i].double_ch[layout_num] = _byte;
      _byte = EEPROM.read(layout_num * 100 + 388 + i);
      if (_byte != 255) b[i].double_toggle[layout_num] = _byte;
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
    if (_byte != 255) options[i] = _byte;
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

  for (byte i = 0; i < NUM_SLIDERS; i++) {
    _byte = EEPROM.read(360 + i);
    if (_byte != 255) a[i].pedal_min = _byte;
    _byte = EEPROM.read(362 + i);
    if (_byte != 255) a[i].pedal_max = _byte;
  }
}

void clear_EEPROM() {
  EEPROM.begin(512);
  for (byte i = 0; i < 350; i++) {
    EEPROM.write(i, 255);
    EEPROM.commit();
  }
}

int iter = 0;
void Check_User_Input() {
  if (b[iter].ext_MIDI_On){
    b[iter].tick(b[iter].ext_MIDI_On);  // standard button check   
  }
  else b[iter].tick();  // standard button check
  b[iter].button_check(!b[iter].isIdle());  // button check for snap
  if (iter < NUM_SLIDERS) a[iter].check_pot();
  r[iter % 2].update_rotary();
  iter++;
  if (iter == NUM_BUTTONS) iter = 0;
}

void setup() {
  // clear_EEPROM();
  setup_EEPROM();
  setup_Buttons();
  init_LEDS();
  setup_MIDI();
  setup_display();
}


void loop() {
  USB_MIDI.read();
  SERIAL_MIDI.read();
  Check_User_Input();
}

long unsigned _now_micro = micros();

void loop1() {
  Display_Handler();
  if (matrix_brightness < 20)  delayMicroseconds(37);
  if (millis() - _now > scrolling_speed / 2) {
    disp.inc_scroll();
    _now = millis();
  }

  if (showing_page && millis()-_now_page > 800) {
    showing_page = LOW;
    disp.build_text(disp.text_len, disp.data_text);
  }
}
