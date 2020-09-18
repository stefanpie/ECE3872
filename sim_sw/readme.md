# Air Sequencer Software Simulation

This directory holds the code for our software simulation of the Air Sequencer.
The software simulation was written in C to most closely align with the C code that would be written for the Arduino Mega which is the platform we plan to use.
The code that would run on the device is only in one file named `sequencer.c`.
A state machine for our device can be found in the `State Machine Air Sequencer.png` image file along with its source document exported from draw.io.
Additional code was written to mock some features of the Arduino software library; that code can be found in `sim_driver.c` and `sim_driver.h`.
To build and run the project, only a standard C compiler (ex. gcc) is needed. For our development, we have included a `build_and_run.cmd` script file to do this task.
The build command in this script can be used as a reference for building on other machines as well.

