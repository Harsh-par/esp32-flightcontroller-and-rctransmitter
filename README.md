# ESP32 Quadcopter Flight Controller

A work-in-progress **quadcopter flight controller** using an **ESP32**, with IMU, barometer, and magnetometer integration. The flight controller communicates with the custom transmitter using the **ESP-NOW protocol**. PID-based control loops maintain stable flight at a 500 Hz loop rate.

---

## Features
- 4-motor control with RS2205 2300 kV motors and LittleBee 30A ESCs  
- Wireless communication with ESP-NOW between flight controller and transmitter  
- Direct register access for MPU6050 and QMC5883 sensor configuration  
- PWM generation for ESCs via GPIO  
- Timer-based control loop for deterministic updates  
- Custom 3D printed quadcopter frame and controller  
- FreeRTOS on ESP32 to leverage dual cores (for controller tasks)

---

## Components Used
| Component | Description |
|-----------|-------------|
| RS2205 2300 kV | Brushless motors (x4) |
| LittleBee 30A ESC | Electronic speed controllers (x4) |
| MPU6050 | Gyroscope + accelerometer |
| BMP390 | Barometer for altitude |
| QMC5883 | Magnetometer for heading |
| ESP32 | Flight controller and transmitter MCU |

---

## Gallery

![Quadcopter](quadcopter_1.jpg)
![](quadcopter_2.jpg)

![Controller](controller_1.jpg)
![](controller_2.jpg)

