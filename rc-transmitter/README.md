# RC Transmitter Firmware

## Overview
This folder contains the firmware for the custom RC transmitter. It runs on an ESP32 microcontroller and communicates with the flight controller using ESP-NOW. The firmware reads joystick and button inputs, drives an OLED display, and manages a precise control loop for responsive drone control.

## Features
- I2C communication with OLED display for status and feedback
- GPIO input with pull-ups for buttons
- ADC reads for joystick axes
- Timer-based deterministic control loop for precise signal generation
- PWM output for servo actuation via GPIO
- ESP-NOW protocol to communicate with the flight controller
- FreeRTOS task scheduling to leverage dual cores of the ESP32

## Hardware
- ESP32 microcontroller
- Joysticks, buttons, and OLED display
- Custom 3D printed enclosure for comfortable use

## Firmware Highlights
- Multi-tasking using FreeRTOS: separate tasks for transmitting data and displaying data using esp32's dual core
- GPIO and ADC used for input capture from joysticks and buttons
- I2C used for OLED display communication
- ESP-NOW protocol for low-latency wireless communication with flight controller

