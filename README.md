## Bluetooth Door Lock
 A NRF52840 Door Lock

# How it works
The whole lock is controlled by an Adafruit NRF52840 express, which supports Bluetooth Low Energy. Using the Bluefruit Connect app by Adafruit, the board checks whether the correct code was sent or not. Instructions are then sent to a 28 BYJ-48 stepper motor controller, opening or closing the 3D-printed rack and pinion depending on a boolean.

# Wiring Diagram:
Adafruit NRF52840:
+------------------------------------+
|                                    |
|  [JST Port] ---- (3.7V LiPo)       |
|                                    |
|  Pin 10 ------- Stepper Controller STEP  |
|  Pin 11 ------- Stepper Controller DIR   |
|  Pin 12 ------- Stepper Controller ENABLE|
|  Pin 13 ------- Stepper Controller MS1   |
|  GND ---------- Stepper Controller GND   |
+------------------------------------+

Stepper Controller:
+-------------------------------------------+
|                                           |
|  A+  ----- Stepper Motor Wire 1           |
|  A-  ----- Stepper Motor Wire 2           |
|  B+  ----- Stepper Motor Wire 3           |
|  B-  ----- Stepper Motor Wire 4           |
|                                           |
|  VIN ------ 9V Battery Positive (+)       |
|  GND ------ 9V Battery Ground (-)         |
|                                           |
+-------------------------------------------+


# The simple STL file is in the repo titled doorLock.stl 
