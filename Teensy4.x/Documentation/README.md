# Documentation for Teensy 4/4.1 CPU Daughterboard
 
www.hamradioworkbench.com has designed and sells a general purpose development
board for Microcontrollers. They have provided the designs for both the CPU
Daughterboards as wells as the addon Prototype board.

This directory contains the documentation for this design. Including: 
- Schematic (pdf format) 
- Pinmapping between the Benchduino pins and the pins on the Microcontroller. 
            to multiple locations on the Benchduino. Please use this pinmapping 
            document along with your favorite pinout diagram to understand which 
            pins could cause
            conflicts.

*IMPORTANT NOTE ON POWER*
This microcontroller has 3 jumpers at the top of the board that allow you to select
which power is fed to the board *AND* whether in the case where the processor is
being powered by USB, the 5V is passed back to the Benchduino motherboard.

NOTE: Generally it is not recommended to power the processor from USB *and* also via
the 5v/3v/Vin power. 

Vin: This pin should generally not be jumpered unless you know what you are doing. When
jumpered, it provides power to the processor. The only problem is that the Benchduino
motherboard *does not* provide power to this pin. It is left floating. So if you want to
use this pin, perhaps because you are using a battery, you *must* provide your own power
to this pin.

5+: This pin is both input and output. When the motherboard of the benchduino is powered
(i.e. a power source is plugged into the barrel connector) 5V is delivered to the
daughterboard. However, in the case where the processor is being powered by the USB
connection, the 5V from the USB is distributed *to* the benchduino motherboard and
used to power various devices you might have. This duo nature probably means you want to
always jumper this one or think about disabling the power wire on your USB cord.

However...
you might want to consider removing this jumper if you are powering the processor
by USB and *do not* the peripherals driven off this power source because of the current
drain. In such a case, remove this jumper and power the Benchduino via the barrel
connector.

3.3V: This is also an input pin to the daughterboard. You want to jumper this
one if you are powering the processor on 3.3V and switch the peripheral voltage rail
to 3.3v.

There is absolutely no warranty here. Use this at your own risk!



73
Mark Hatch
AJ6CU
 
