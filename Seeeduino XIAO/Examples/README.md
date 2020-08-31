# Example for Seeeduino Xiao Benchduino CPU Daughterboard
 
 
www.hamradioworkbench.com has designed and sells a general purpose development
board for Microcontrollers. They have provided the designs for both the CPU
Daughterboards as wells as the addon Prototype board.

This directory contains the code example (typically derived from the original 
Hamvention code of the published on the hamradioworkbench site)

There is absolutely no warranty here. Use this at your own risk!

General Comments:

1. The pin assignment on the Seeeduino Xiao is substantially different than
the defaults of the Benchduino. For example, you can not use D2, D3, and D4 because
D4 is used for SDA. There is also a conflict on the default button pins and there is
insufficient pins to drive all 4 LED's separately. There was also insufficient pins
to support A0 (used for the pot).

 
2. The Seeeduino Xiao also does not support  Serial1, only Serial. Consequently, large 
portions of the code have been comment out. 

3. There was also some issue with interupts that forced me to move D2 (PinA on the 
encoder) to another pin. Did not further investigate this issue.



73
Mark Hatch
AJ6CU