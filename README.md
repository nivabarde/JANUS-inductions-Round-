# JANUS-inductions-Round-

# Team JANUS CanSat 2025-2026 – Avionics Subsystem Solutions

This repository contains the solutions for the final round problems of the Team JANUS CanSat 2025-2026 competition. The solutions cover:

1. Flight Software – Arduino code for GPS parsing and flight state machine.
2. Ground Control System – Web interface for live plotting of sensor and GPS data using Chart.js.
3. PCB Design – KiCad PCB layout for STM32-based board with BMP290 sensor.




Solution 1: Flight Software

Objective:
Set up the Quectel L89HA GNSS Module to receive GPS data, print it, and implement a flight state machine.

Features Implemented:

* Reads and parses NMEA sentences (GNRMC & GNGGA).
* Prints time, latitude, longitude, and altitude to the Serial Monitor.
* Implements flight state machine with six states:
* Assumes a normal flight path (ascend → apogee → descend).

Usage:

1. Connect the Quectel L89HA GNSS module to the Arduino.
2. Upload the  FlightSoftware.ino sketch.
3. Open Serial Monitor to see live GPS data and state transitions.


Solution 2: Ground Control System

Objective:
Create a web-based interface to read Arduino sensor data via Web Serial API and plot it live.

Features Implemented:

* Live 2D plotting of sensor values using Chart.js.
* 3D-like visualization of **latitude, longitude, and altitude** with color-coded altitude.
* Supports both **simulated data** and live Arduino data.



 Solution 3: PCB Design

Objective:
Design a PCB in KiCad for an STM32-based board with BMP290 sensor and USB-B power port.

Features Implemented:

* Correct component placement and routing according to schematic.
* Separate trace widths for **power** and **signal lines**.
* **Ground plane** on bottom layer with appropriate vias.
* Fully manual routing (no auto-router used).

