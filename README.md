# MicrocontrollersProject
 Hands-free deodorant dispenser

## Parts List
- 3x 1µF capacitors
- 1x 10µF capacitor
- 1x 2x3 header
- 1x 1x3 header
- 1x 1x4 header
- 1x transistor
- 1x Q3 crystal
- 2x 22 Ohm resistors
- 3x 10k Ohm resistors
- 2x buttons
- 1x 3 prong switch
- 1x AT90USB162 chip
- 1x USB micro B port
- 1x servo
- 1x Sharp GP2D02 IR sensor

## User Instructions
Flip the switch to on. Position yourself directly in front of the sensor. To tirgger the servo, move within 8-13 cm of the sensor. To release the servo, don't be within 8-13cm of the sensor.

## Code Overview
Pinout:
- PC7 - "on/off" switch
- PB5 - sensor output
- PB6 - sensor Vin
- PB7 - servo

GetDist() reads the data value from the IR sensor. The biggest roadblock was figguring out that the IR sensor was open drain. Otherwise the function just follows the Sharp GP2302 datasheet.

The main() function configures the pinout and initializes the PWM timer (clear OC0A at match, fast PWM mode, top defaults to 0xFF, prescaler = 64). The while loop waits until the switch is positioned at "on," and then moves the servo to the appropriate position if the user is within range or out of range. If the user is not detected by the sensor, the servo is turned off.
