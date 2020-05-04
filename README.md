# VNWheel
This is open source force feedback wheel base on Arduino 1.8.10, future for teensy 4.0 600MHz :O!!!!!!

The code use some libraries and files from other projects like:
- adapt-ffb-joy 
- ForceFeedback-core-library
- kaskus forums
- microchip forums
- filter

https://github.com/MartinBloedorn/libFilter

It has not tested yet.

Usage:
- pin0,1,2 for encoder
- pin 9, 10 for Pulse/dir; PWM+-
- Modify WheelConfig to match your hardware setup

bool initialRun = true; => run 1st time for calculate maxPositionChange, maxVelocity, maxAcceleration. after that you can set to false and assign directly to these variables (uncomment codes)

    Wheel.encoder.maxPositionChange = 1151;
    Wheel.encoder.maxVelocity  = 72;
    Wheel.encoder.maxAcceleration = 33;


PS: stm32 version is in development phase:

https://youtu.be/qPKXuQauSpo

https://youtu.be/B8nHdp_dTPk
