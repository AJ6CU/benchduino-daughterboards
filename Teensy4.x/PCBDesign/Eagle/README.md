# Eagle files for the Teensy 4.x Series MCU Daughterboard

This directory includes the Eagle files for a design of a Benchduino Daughterboard
(see https://www.hamradioworkbench.com ) for the Teensy 4.x series of MCU.

This has only been tested on the 4.1 version, and since the 4.0 is just a shortened
version of the 4.1 with the same pinout, it *should* work too. But it has not been tested.

This folder includes:
- Teensy4.1V3.sch - schematic
- Teensy4.1Ve.brd - board layout

It is possible that you will need the Eagle library for Teensy. I found it

https://forum.pjrc.com/attachment.php?attachmentid=20116&d=1589570415

In making this design, my philosophy was to try to make sure most Benchduino Pins were 
connected to appropriate pins if possible.

This means that some pins (e.g. SDA, and A4) are both active and map to the same pin 
on the Teensy. So don't assume that because you have two pins, that they are separate. 
The Teensy pin guide is your friend to make sure you don't use the same pin twice
for a different purpose.

Feedback is appreciated!

73
Mark
AJ6CU




 
