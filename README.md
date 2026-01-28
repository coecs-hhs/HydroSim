# HydroSim – Hardware Branch

This branch contains all files related to the **hardware side** of the HydroSim project.

The focus of this branch includes:
- Raspberry Pi 
- Siemens PLC (TIA Portal)
- QT-C++ simulation

## Raspberry Pi

This branch contains a folder with:
- Raspberry Pi code
- Environment variables
- Requirements for installation using pip

[`Raspberry Pi Documentation`](Raspberry%20Pi/docs/Raspberry_Pi_Documentation.md)

This document explains the setup, configuration, and usage of the Raspberry Pi within the HydroSim system.

## Siemens PLC (TIA Portal)

This branch contains a folder with:
- A **Siemens PLC TIA Portal `.zap` project file**
- Supporting documentation related to the PLC configuration

[`Siemens PLC Documentation`](PLC/docs/plc.md)

These files are intended for use with **Siemens TIA Portal version 15.1+** and describe the PLC logic, hardware configuration, and integration with the HydroSim system.

## QT-C++ simulation

In the folder **HydroSim-QT-Simulation** you can find the following:
- QT (QML) code for a visual simulation of the HydroSim server 
- C++ code for the backend of the QT environment
- Supporting documentation

This simulation/GUI is created to test the hardware side of the HydroSim project *without* a server connection.
To use the simulation, you can read about it in the [`README`](HydroSim-QT-Simulation/README.md). 
To further develop the program, you can find more information in the [`Developer Guide`](HydroSim-QT-Simulation/docs/Developer%20Guide.md).

---

This branch is intentionally separated from the software and simulation logic to keep the project structure clear, organized, and maintainable.

