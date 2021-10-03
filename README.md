![open.control](https://github.com/KBLiveSolutions/open.control/blob/main/assets/images/logo_big.png)

# open·control firmware

### Download the content of this Repository
Click on **[Code: Download ZIP]** or click [here](https://github.com/KBLiveSolutions/open.control-firmware/archive/refs/heads/main.zip)

***If you have already installed the Arduino IDE and Libraries, you can skip to [section 4](#4-open-the-firmware-file).***
## First time install:

### 1. Install Arduino

Download and install the [Arduino IDE](https://www.arduino.cc/en/software/)

### 2. Replace Arduino's boards.txt file:

OSX:
* Go to Applications, right click on Arduino, and show content
* Go to Java/hardware/arduino/avr
* Replace the existing boards.txt with the one included in the open.control-firmware.zip

Windows:
* Go to c:\program files\Arduino\hardware\arduino\avr
* Replace the existing boards.txt with the one included in the open.control-firmware.zip

### 3. Install the Libraries:

* Download the following libraries: [Bounce2](https://github.com/thomasfredericks/Bounce2/archive/refs/heads/master.zip), [ResponsiveAnalogRead](https://github.com/dxinteractive/ResponsiveAnalogRead/archive/refs/heads/master.zip), [FastLED](https://github.com/FastLED/FastLED/archive/refs/heads/master.zip), [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB/archive/refs/heads/master.zip), [TimerThree](https://github.com/PaulStoffregen/TimerThree/archive/refs/heads/master.zip)
* Open Arduino
* Go to the Sketch menu Include Library -> Add .ZIP Library
* Choose each library one by one

### 4. Open the firmware file


* Open in Arduino the file *open_control_firmware.ino* contained in the unzipped folder
* If being asked to create a folder, click Yes
* Make sure open·control is connected
* In Arduino Tools menu, select : 
    * Board -> open.control
    * Port: 
        * OSX: /dev/cu.usbmodemMIDI1
        * Windows: /COM3

## You are now ready to upload the firmware, just click on the Arrow in the toolbar, or press Cmd-U



