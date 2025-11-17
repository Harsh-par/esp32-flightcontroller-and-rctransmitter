# Flight Controller Firmware

## Overview
This folder contains the firmware for the custom quadcopter flight controller. It runs on an ESP32 microcontroller and implements PID-based attitude control for stable flight.

## Features
- Sensor integration over I2C (MPU6050 IMU, BMP390 barometer, QMC5883 magnetometer) and ADC
- Low-level register access for sensor configuration and calibration
- 500 Hz control loop using hardware timers and interrupts
- PWM signal generation for ESCs with GPIO
- PID-based control for roll, pitch, and yaw
- Communication with RC transmitter using ESP-NOW protocol

## Hardware
- ESP32 microcontroller
- MPU6050 IMU, QMC5883 magnetometer, BMP280 barometer
- ESCs for brushless motors
- Simple 2-layer PCB holding sensors, buck converters, and MCU

## Firmware
- Direct register writes to MPU6050 and QMC5883 for sensor setup, scaling, and filtering
- GPIO driven PWM generation for precise motor control
- Timer based scheduling for deterministic control loop
- ADC reads for additional sensor inputs

