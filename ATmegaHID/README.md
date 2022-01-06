# USE ATmega32U4 AS A HID
In this project the main file is HIDMouse.ino

First we used Gyroscope (MPU6050 is Accelero+Gyro) to convert the angle we move to distance the cursor will move.
So there are [I2Cdev and MPU6050 Library](https://github.com/leech001/MPU6050) for simply uses.

Then we will use Mouse.h to send [report](https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/) to computer, but there's a problem which is this Mouse lib cannot scroll left and right and there's [HMouse](https://github.com/nikita-tomilov/HMouse) which improv it!! Thanks a lot to Nikita!!

