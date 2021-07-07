# ATiny QC 3 Controller

## NOTE: **NOT WORKING**, or at least I have not been able to test it working, more info below

## What is this
This is a controller for chargers that implement the Quick Charge 3.0 standard.
It allows the charger to output a variable voltage by mimicking this standard using the ATiny's GPIO and a few resistors.
This implementation also uses a 4 digit display to help with visualizing the current voltage.

## Hardware

### Final Build
- [ATiny45](https://www.microchip.com/wwwproducts/en/ATtiny45)
- [TM1637 4-Digit Display](https://www.amazon.com/HiLetgo-Digital-Segment-Display-Arduino/dp/B01DKISMXK/ref=sr_1_2?dchild=1&keywords=4+digit+display&qid=1625625825&sr=8-2)
- [DC to DC Converter](https://www.digikey.com/en/products/detail/VR05S3V3/1470-VR05S3V3-ND/13147710?itemSeq=365506420) **NOT IMPLEMENTED**
  - Input Range: 3.3v - 20v
  - Output : 3.3v - 5v
- 1x Tactile Switch
- 1x 10μF 25V capacitor
- 10kΩ Potentiometer
- Some resistors
  - 2x 100Ω
  - 2x 4700Ω
  - 2x 1500Ω
  - 1x 10kΩ
  - 1x 5kΩ
  - 1x 1kΩ
### For Arduino ISP programmer
- [Arduino Uno](https://www.arduino.cc/en/Main/arduinoBoardUno&gt;)
- 3x Leds
- A bunch of wire
- Toggle switch ( for convenience )
- Some resistors
  - 3x 100kΩ

## Software
- [Microchip Studio](https://www.microchip.com/en-us/development-tools-tools-and-software/microchip-studio-for-avr-and-sam-devices)
- [AvrDude](https://www.nongnu.org/avrdude/)
  - Windows binary included in repo

This project was mainly built using Microchip studio on Windows, however, it should be adaptable to other systems / toolchains.

## Building / Flashing

The following info only applies if you use Microchip Studio.

When using the [Arduino ISP programmer](https://www.instructables.com/Program-an-ATtiny-with-Arduino/) ( I did not use the 10μF cap for this ) on Microchip Studio, ensure to change the COM port on the `Post-build event command line` under `Build Events` under the project properties window.

This means that, after a successful build, it should automatically attempt to flash using the Arduino ISP programmer.

Note: This project was made for the ATiny45, the ATiny85 should also work ( Just make sure to also change that in the `Post-build event command line` ), but the ATiny25 will not, as it does not have enough space for the final binary.

## Wiring

The only wiring I will take note of does not involve the Arduino ISP.

|[ATiny PBx Pin](https://raw.githubusercontent.com/SpenceKonde/ATTinyCore/master/avr/extras/Pinout_x5.jpg)|Connection|
|-|-|
|0|Data Minus|
|1|Data Plus|
|2|Potentiometer|
|3|TM1637 DIO Pin|
|4|TM1637 CLK Pin|
|5|Tactile Switch with debouncing capacitor`*`|

`*` Note: PB5 is normally only used as the ATiny reset pin unless permanently programmed to use it as a normal GPIO. However, by using a 10kΩ and 5kΩ resistor in parallel ( equivalent to ~ 3.3kΩ resistor ) on the button, it can still be used as input in a limited range.

### Images

Refer to [Turning a Quick Charge 3.0 charger into a variable voltage power supply by Vince](http://blog.deconinck.info/post/2017/08/09/Turning-a-Quick-Charge-3.0-charger-into-a-variable-voltage-power-supply) for the actual QC3 Circuit, not shown here.

Take note of the final update on this blog post.

![Wiring](Images/Wiring.png)
Shown above is the wiring without and with the larger parts, alongside the Arduino ISP.

## Usage

The only input that the ATiny takes in is the potentiometer and the button.

The potentiometer only works when the digits are flashing, it is used for changing values.

The button has two functions.
 - Long press - Used for confirming values
 - Normal press - Used for switching between 'menu' items

The controller uses a very simple menu system, where the button is used to select different options.

This 'menu' can be visualized as follows.

|# Presses|**MainMenu** [0]|**Voltage Select** [1]|**Accurate Voltage Select** [2]|
|-|-|-|-|
|Long Press|None|Confirm`**`|Confirm`**`|
|1|Toggle LED|**Switch Menu [0]**|Edit digit 0|
|2|**Switch Menu [1]**|None`*`|Edit digit 1|
|3|**Switch Menu [2]**|N\A|Edit digit 2|
|4|None`*`|N\A|Edit digit 3|
|5|N\A|N\A|**Switch Menu [0]**|

`*` This option will reset the selection and go back to the default state

`**` This option will first normalize the given voltage to QC 3.0 standard then return to **MainMenu** [0]

## Resources

- [Turning a Quick Charge 3.0 charger into a variable voltage power supply by Vince](http://blog.deconinck.info/post/2017/08/09/Turning-a-Quick-Charge-3.0-charger-into-a-variable-voltage-power-supply) 
  - Most of the QC 3.0 Stuff was done by him

 - [Arduino ISP programmer by randofo](https://www.instructables.com/Program-an-ATtiny-with-Arduino/) 
   - Roughly followed this guide
 - [attiny-tm1637-library by lpodkalicki](https://github.com/lpodkalicki/attiny-tm1637-library)
   - Heavily modified this library for my situation

## Not worky, I don't think

Unfortunately, the charger that I bought does not seem to actually be QC 3.0. My only clue is that it does not output 5V on standby, and, from what I understand, the QC standard makes chargers output 5V on standby. Regardless, this does not let me power the ATiny from the same port.

Nevertheless, everything else, like the menu system and the potentiometer, seems to be working just fine.