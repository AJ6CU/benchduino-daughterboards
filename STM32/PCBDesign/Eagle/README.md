# Eagle files for the STM32 "BluePill" Series MCU Daughterboard

This directory includes the Eagle files for a design of a Benchduino Daughterboard
(see https://www.hamradioworkbench.com ) for the STM32 "BluePill" series of MCU.


This folder includes:
- STM32BluePill.sch - schematic
- STM32BluePill.brd - board layout

It is possible that you will need the Eagle library for STM32. I found it with a little
googling.

In making this design, my philosophy was to try to make sure most Benchduino Pins were 
connected to appropriate pins if possible.

This means that some pins (e.g. SDA, and A4) are both active and map to the same pin 
on the STM32. So don't assume that because you have two pins, that they are separate. 
The STM32 pin guide is your friend to make sure you don't use the same pin twice
for a different purpose.

Feedback is appreciated!

73
Mark
AJ6CU




 
