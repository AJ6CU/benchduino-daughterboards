# ESP-S2-Saola CPU Daughterboard for Benchduino
 
www.hamradioworkbench.com has designed and sells a general purpose development
board for Microcontrollers. They have provided the designs for both the CPU
Daughterboards as wells as the addon Prototype board.

This directory contains the CPU Daughterboards for the ESP-S2-Saola processor.

Subdirectories include:
- Gerbers - this directory includes a zip file that you should be able to upload to
            your favorite Fab house.
- PCBDesign - Most of the designs are done with KiCad. A few of the earlier ones
            were done with Eagle. The subdirectory will indentify which tool was used. 
- Documentation - This directory contains a schematic (pdf format) and pinmapping between
            the Benchduino pins and the pins on the Microcontroller. Please note that
            some physical pins on the Microcontroller might be mapped to multiple 
            locations on the Benchduino. Please use this pinmapping document along
            with your favorite pinout diagram to understand which pins could cause
            conflicts.
- Examples - Generally I have provide a code example (typically derived from the
            original Hamvention code of the published on the hamradioworkbench site).
            Make sure you read the "README" file in this directory as often there are
            significant porting issues that you might need to be aware of in your
            own applications.

There is absolutely no warranty here. Use this at your own risk!


73
Mark Hatch
AJ6CU

