# Proliant Arduino code

This code is to be flashed onto arduino to control fans

## Compability

This will work with no modifications on all atmega328 based boards.
If someone wants to add compabilty to other chips then go ahead :D

## Todo

The serial part is bit bloated, the serial.printlns can be removed or I could stop using Strings altogether to make the code smaller but it works reliabliy as is.
If anybody has free time then they can just make the arduino code parse commands like this '[fan number],[fan speed%]'. I might do it myself sometime but now too busy with other stuff.