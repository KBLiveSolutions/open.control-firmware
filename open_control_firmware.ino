// #include <Arduino.h> 
#include <Bounce2.h>
#include <EEPROM.h>
#include <ResponsiveAnalogRead.h>
#include <FastLED.h>
#include <MIDIUSB.h>
#include <TimerThree.h>

byte const NUM_LAYOUT = 3;
byte current_layout = 0;

byte const NUM_SLIDERS = 2;

byte const NUM_BUTTONS = 6;
byte const MAX_CHAR = 20;

int matrix_brightness = 200;
byte toggle_Col = 13; // PC7
byte toggle_Row = 6;  // PD7

int hold_button_time = 800;

byte external_MIDI_control[NUM_BUTTONS+NUM_SLIDERS] = {1, 2, 3, 4, 5, 6, 1, 1};
byte external_MIDI_channel[NUM_BUTTONS+NUM_SLIDERS] = {1, 1, 1, 1, 1, 1, 1, 1};
byte external_MIDI_type[NUM_BUTTONS+NUM_SLIDERS] = {0, 0, 0, 0, 0, 0, 0, 0};

#define NUM_LEDS 6
CRGB leds[NUM_LEDS];
int BRIGHTNESS = 50;
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

unsigned long scrolling_speed = 200;

const int b_pins[NUM_BUTTONS] = {11, 3, 2, 9, 10, 5};
#define LED_PIN A2

const int a_pins[NUM_SLIDERS] = {A1, A0};
