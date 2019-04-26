# FlexRTE-X

This repository will contain the new and improved version of the Flex Runtime Environment (renamed from FlexVM Runtime Environment).

It will be a complete rewrite of the old version with a fully object oriented structure and multiple target platforms in mind. The size will be held to a minimum to support less powerful devices such as microcontrollers so there will be no usage of any of the STL functions normally used in C++ projects on machines with for example Windows. So no smart pointers and any of this stuff. :(

**The goal of this project are following:**
- Providing an improved version of the FlexRTE with better performance and more features.
- Writing smaller and more efficient code using pointers. 
- Learning about writing C++ projects for multiple platforms.
- Getting an "usable" bytecode interpreter for microcontrollers.

**Features:** (will constantly be updated)
- Complete FlexRTE instruction set
- Execution of multiple binaries
- Stack- and Heap-Memory allocations
- Virtual Memory*

*\* = not implemented yet*

**These platforms will get supported for sure:**
- Windows
- Teensy 3.2 and up
- ESP32

In the future these platforms might be supported:
- Arduino
- Linux
- Android
