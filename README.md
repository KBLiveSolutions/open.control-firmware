<p align=center><img src="https://github.com/KBLiveSolutions/open.control/blob/main/assets/images/logo_big.png" alt="logo" width="400"/></p>

# open·control firmware

Version 1.1

## To install :

- Download the file **open_control_firmware_1.1.ino.uf2**
- Connect open·control ***while holding Button 1***. 
- A flash drive named **RPI-RP2** will appear.  
- Drag and drop the **.uf2** file inside it.  
- The board will reboot with the new firmware installed.  

## To modify:
- Install the Arduino IDE

Install the following libraries:
- Adafruit Neopixel 
- OneButton.h
- MIDI.h
- Adafruit_TinyUSB.h

Modify the code as needed and upload to the board.

You also need to install the [Earle Philhower Arduino port for the RP2040](url=https://github.com/earlephilhower/arduino-pico).

# Concept

## Buttons

Each button can send Short, Long and Double presses thanks to the OneButton library.
Each of these presses can send either Note, Control Change, or Program Change messages, and this across 3 pages.
Thus, for 1 button, we have:

|      Page 1   | Type           |  Control      |  Channel   |
|--------|------|---------|---------|
|  Short Press  |  Note, CC, PC  |    0 to 127   |  1 to 16   |
|  Long Press   |  Note, CC, PC  |    0 to 127   |  1 to 16   |
|  Double Press |  Note, CC, PC  |    0 to 127   |  1 to 16   |


|      Page 2   | Type           |  Control      |  Channel   |
|--------|------|---------|---------|
|  Short Press  |  Note, CC, PC  |    0 to 127   |  1 to 16   |
|  Long Press   |  Note, CC, PC  |    0 to 127   |  1 to 16   |
|  Double Press |  Note, CC, PC  |    0 to 127   |  1 to 16   |


|      Page 3   | Type           |  Control      |  Channel   |
|--------|------|---------|---------|
|  Short Press  |  Note, CC, PC  |    0 to 127   |  1 to 16   |
|  Long Press   |  Note, CC, PC  |    0 to 127   |  1 to 16   |
|  Double Press |  Note, CC, PC  |    0 to 127   |  1 to 16   |


## LEDs

For the LEDs, the only type available is Control Change. Thus we have

|      Page 1   | Type           |  Control      |  Channel   |
|---|---|---|---|
| LED           |  CC  |    0 to 127   |  1 to 16   |


|      Page 2   | Type           |  Control      |  Channel   |
|---|---|---|---|
| LED           |  CC  |    0 to 127   |  1 to 16   |


|      Page 3   | Type           |  Control      |  Channel   |
|---|---|---|---|
| LED           |  CC  |    0 to 127   |  1 to 16   |

When opencontrol receives a CC message, it is compared to the CC number of each LED for the current page.
If it is similar, then the value received lights up the LED to the corresponding color.

