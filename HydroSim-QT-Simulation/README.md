# HydroSim-GUI: User Guide

## Overview
This project simulates the **HydroSim server** for testing hardware behavior and communication **without requiring a real server connection**.  
The project is implemented in **C++** with **Qt/QML** integration and visualizes devices on top of zone-based background images.

Devices such as **Pumps** and **Valves** are displayed as interactive buttons that can be toggled directly from the UI.  
All data structures and behavior closely resemble the real HydroSim server.

If looking to contribute or add to the project, see the [Developer Guide](docs/Developer%20Guide.md).

---

## Prerequisites
Before running the project, ensure you have:

1. **Qt 6.x or 5.x** installed (Qt Creator recommended)
   - Required modules: `QtCore`, `QtQuick`, `QtQml`
   - Optional but recommended: `QtMqtt`  
     *(Can be enabled via the Qt installer — you may need to restart the installer to see it.)*
2. A **C++17 compiler** or higher
3. Basic knowledge of **Qt/QML projects**

---

## Project Structure
```bash
ProjectRoot/
│
├─ devices/ # Device-related classes  
│ ├─ Device.h
│ ├─ Pump.h/cpp
│ ├─ Valve.h/cpp
│
├─ main.cpp # Application entry point
├─ backend.h/cpp # Backend logic (C++ ↔ QML bridge)
├─ mqttclientwrapper.h/cpp # MQTT communication logic
└─ qml/
  └─ Main.qml # Main QML UI
```


### Components
- `Device` – Base class containing shared properties (`id`, `zone`, `type`, `position`)
- `Pump` – Extends `Device` with `status`, `flow`, `quality`, `speed`
- `Valve` – Extends `Device` with `status`, `flow`, `quality`
- `backend` – Exposes devices and control logic to QML
- `mqttclientwrapper` – Handles MQTT communication
- `Main.qml` – Visual UI with background images and interactive device buttons

---
## Building the Project

### Using Qt Creator (optional)
> The project is primarily built using CMake, but Qt Creator **can** be used if preferred. (This isnt tested!)

1. Open **Qt Creator**
2. Select **File → Open File or Project**
3. Open the root `CMakeLists.txt`
4. Configure the kit (Desktop Qt, C++17)
5. Build and run the project

---

### Using Command Line (recommended)

```bash
# Make sure you are in the project root
cd HydroSim-GUI

# Create build directory
mkdir build && cd build

# Configure project (CMake 3.16+)
cmake ..

# Build project
make            # or mingw32-make on Windows

# Run application (example path)
./bin/HydroSimGUI
```
---
## MQTT Configuration

This application communicates using **MQTT over TLS**.  
For the connection to work correctly, the **MQTT certificates must match the ones currently expected by the application**.

---
### Certificate Requirements
The application loads MQTT certificates from the **Qt resource system** (`resources.qrc`).  
This means:

- The **certificate files must exist**
- The **file names must match exactly**
- The certificates must be **valid for the configured MQTT broker**

If the certificate names do not match, the application will fail to connect to MQTT.

---

### Expected Certificate Files
The following certificate files are expected to be present and referenced in `resources.qrc`:

- `ca.crt` – Certificate Authority certificate
- `client.crt` – Client certificate
- `client.key` – Client private key

> ⚠️ **Important:**  
> The file names are case-sensitive and must match exactly.

---

### Changing Certificates
If you want to use different certificates:

1. Replace the existing certificate files **using the same file names**,  
   **OR**
2. Update the file names in:
   - `resources.qrc`
   - Any C++ code that loads the certificates (MQTT setup)

- Example (`resources.qrc`):
    ```xml
    <qresource prefix="/certs">
        <file alias="ca.crt">certs/ca.crt</file>
        <file alias="client.crt">certs/client.crt</file>
        <file alias="client.key">certs/client.key</file>
    </qresource>
---
## Running the Project

When the application starts:

- Main.qml is loaded as the main UI

- A background image is displayed (main page, overview, or zone-specific)

- Devices are fetched dynamically using:

        backend.getDevices()

- Each device is visualized as a button positioned relative to the background image

- Only devices belonging to the selected zone are shown

---
### Device Interaction
- Each device is represented by a button:
    - Green → device is OFF
    - Red → device is ON

- Clicking a button toggles the device state:
        
        backend.toggleDevice(modelData)

- Button text and color update automatically via Q_PROPERTY bindings

---
### UI Navigation

- A ComboBox at the top of the screen allows switching between:
    - Main page
    - EPANET overview
    - Individual zones (Zone 0 – Zone 4)
- Changing the selected item updates the background image and visible devices

---
### Main.qml Behavior Summary

- Background images are loaded from Qt resources (qrc:/images/...)

- Devices are positioned using normalized coordinates (position.x, position.y)

- Buttons scale dynamically with the background image size

- The UI reacts automatically to backend state changes

---
## Tips
- Always emit signals in setters for QML to react to property changes.

- Keep base class (Device) properties constant to prevent accidental modification.

- Use Q_PROPERTY for all properties that should appear in QML.

- Normalized positions (0.0 – 1.0) make the UI resolution-independent

---