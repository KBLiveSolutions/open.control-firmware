![open.control](https://github.com/KBLiveSolutions/open.control/blob/main/assets/images/logo_big.png)

# open·control firmware

## This is the firmware file for open·control

## If you have already installed the Arduino IDE and Libraries, you can skip to section 4.

## First time install:

### 1. Install Arduino

Download and install the [Arduino IDE](https://www.arduino.cc/en/software/)

### 2. Replace Arduino's boards.txt file:

OSX:
* go to Applications, right click on Arduino, and show content
* go to Java/hardware/arduino/avr
* replace the existing boards.txt with the one included in the open.control-firmware.zip

Windows:
* go to c:\program files (x85)\Arduino\hardware\arduino\avr
* replace the existing boards.txt with the one included in the open.control-firmware.zip

### 3. Install the Libraries:

* Download the following libraries: [Bounce2](https://github.com/thomasfredericks/Bounce2/archive/refs/heads/master.zip), [ResponsiveAnalogRead](https://github.com/dxinteractive/ResponsiveAnalogRead/archive/refs/heads/master.zip), [FastLED](https://github.com/FastLED/FastLED/archive/refs/heads/master.zip), [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB/archive/refs/heads/master.zip), [TimerThree](https://github.com/PaulStoffregen/TimerThree/archive/refs/heads/master.zip)
* Open Arduino
* go to the Sketch menu Include Library -> Add .ZIP Library
* choose each library one by one

### 4. Open the firmware file

* Download the content of this [Repository](https://github.com/KBLiveSolutions/open.control-firmware/archive/refs/heads/main.zip)
* Make sure open·control is connected
In Arduino Tools menu, select :
* Board -> open.control
* Port: /dev/cu.usbmodemMIDI1

## You are now ready to upload the firmware, just click on the Arrow in the toolbar, or press Cmd-U



