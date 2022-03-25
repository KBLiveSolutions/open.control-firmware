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
#define FIRMWARE_MINOR_VERSION 5

// 1.5: fixed LED not updating after long press

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
#include "WEBUSB.h"
#include "EEPROM.h"


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
  check_editor();
}

void setup() {
  // clear_EEPROM();
  setup_EEPROM();
  setup_Buttons();
  setup_LEDS();
  setup_MIDI();
  setup_display();
  setup_webusb();
}

void loop() {
  USB_MIDI.read();
  SERIAL_MIDI.read();
  Check_User_Input();
  usbweb_loop();
}


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
