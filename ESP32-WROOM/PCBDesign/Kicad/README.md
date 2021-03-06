# KiCad files for the ESP32-WROOM MCU Daughterboard

This directory includes the KiCad files for a design of a Benchduino Daughterboard
(see https://www.hamradioworkbench.com ) for the ESP32-WROOM.

This directory contains a zip file of a KiCad archive of the project. You should be
able to expand it on your system and open up the project file and start modifying it.

In making this design, my philosophy was to try to make sure most Benchduino Pins were 
connected to appropriate pins if possible. 

This means that some pins (e.g. Analog and Digital pins) are both active and map to the 
same physical pin on the ESP32-WROOM. So don't assume that because you have two pins, 
that they are separate. The ESP32-WROOM pin guide is your friend to make sure you 
don't use the same pin twice for a different purpose.

In some cases, it was not possible to map
the ideal pin on the ESP32 to the corresponding pin on the Benchduino. For example, GPIO11,
which one would think naturally maps to D11, was not a possible match because
GPIO11 is a special pin used for SPI flashing and so not available to the user. That
is why the photo of the Hamvention demo has that extra jumper wire.

Feedback is appreciated!

73
Mark
AJ6CU




 
