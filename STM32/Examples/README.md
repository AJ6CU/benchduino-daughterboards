# Example for STM32 Benchduino CPU Daughterboard
 
 
www.hamradioworkbench.com has designed and sells a general purpose development
board for Microcontrollers. They have provided the designs for both the CPU
Daughterboards as wells as the addon Prototype board.

This directory contains the code example (typically derived from the original 
Hamvention code of the published on the hamradioworkbench site)

There is absolutely no warranty here. Use this at your own risk!

THERE IS ALSO A VERY IMPORTANT CHANGE THAT IS BECAUSE THE STM32 USES PINS 
D11 and D12 FOR USB AND THE SERIAL CONSOLE. So if you want to just jump D11 and D12 to 
the LED's you need to close the console first (Serial.end();)! My recommendation is just 
to use different pins and put in jumpers and keep the console output.

ALSO NOTE THAT I HAD PROBLEMS GETTING THE OLED LIBRARY WORKING as there appears to be a 
memory leak. I suspect this was because I was using an old version of the STM32 board 
support package....



73
Mark Hatch
AJ6CU