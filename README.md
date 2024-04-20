This file is just a way to visually track the dependencies of the bms1000m class.
So we can at least somewhat identify what classes need to be re-written (contains code that is TI specific)

For files that have not been checked yet, their respective dependency tree will be marked as [UNCHECKED]

bms1000m
    -critical = maybe needs to be changed?
        -interrupt = needs changes
    -can_interface
        -hw_ints = needs changes
        -hw_memmap = needs changes
        -can = needs changes
        -gpio = needs changes
        -interrupt = duplicate
        -sysctl = needs changes
        -can_message = okay
        -can_receiver = duplicate
    -can_receiver
        -can = duplicate
        -can_ports = okay
    -error = okay, but does nothing? (check Library/error.cpp)

Here is a list of all classes identified that are required to be modified:

1) driverlib/interrupt = major changes
2) inc/hw_ints = major changes
3) inc/hw_memmap = major changes
4) driverlib/can = moderate changes / deeper reading
5) driverlib/gpio = major changes
6) driverlib/sysctl = major changes
7) Library/critical = minor changes
8) Library/can_receiver = minor changes

Please open issues for these classes in the issues tab.

rms_inverter and sme_acx1_inverter still have to be checked