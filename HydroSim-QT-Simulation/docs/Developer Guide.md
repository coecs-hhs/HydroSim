# HydroSim-GUI: Developer Guide

## Architecture Overview
The project follows a clear separation of concerns:

- **C++ Backend**
  - Holds all device logic (`Device`, `Pump`, `Valve`)
  - Manages device state and simulation behavior
  - Exposes devices and control functions to QML
  - Handles MQTT communication via `mqttclientwrapper`

- **QML Frontend**
  - Responsible only for visualization and user interaction
  - Displays devices dynamically based on backend data
  - Never contains business logic

This separation keeps the UI flexible and the simulation logic reusable.

---

## Backend ↔ QML Communication
- Devices are exposed to QML as `QObject` instances.
- Properties are declared using `Q_PROPERTY` and updated via signals.
- Device lists are provided through:
  ```cpp
  QList<QObject*> Backend::getDevices();
- Device interaction from QML is handled via:
    ```cpp
    Q_INVOKABLE void toggleDevice(Device* device);  
---
## Device Positioning

Device positions are stored as normalized coordinates (0.0 – 1.0).

- In QML, these values are multiplied by the background image size:
    ```qml
    x: (bg.width - bg.paintedWidth) / 2 + bg.paintedWidth * modelData.position.x
    y: (bg.height - bg.paintedHeight) / 2 + bg.paintedHeight * modelData.position.y
This approach makes the UI resolution-independent and easy to scale.

---
## Zone Handling

Each device belongs to a specific zone (device.zone).

- Zones map directly to background images.
- Devices are shown only when the selected image corresponds to their zone:
    ```qml
    visible: (modelData.zone + 2) === currentImageIndex
The offset allows non-zone pages (main page, overview) to exist before zone views.

---
## State Management

Device state changes must always go through setters.

- Setters must:
    1. Check for value changes
    2. Update the internal variable
    3. Emit the corresponding signal

- Example:
    ```cpp
    void Pump::setStatus(bool status)
    {
        if (m_status == status)
            return;

        m_status = status;
        emit statusChanged(m_status);
    }
This ensures QML stays synchronized with the backend.

---
## MQTT Integration

***MQTT logic is isolated in mqttclientwrapper.***

Backend acts as a bridge between:
- MQTT messages
- Internal device state
- QML updates
- This makes it easy to:
    - Replace the simulator with a real server later
    - Switch MQTT brokers without touching the UI

---
## Adding a New Device Type

To add a new device type (e.g. Sensor):
1. Create a new class inheriting from Device
2. Add required Q_PROPERTY fields
3. Implement setters with signals
4. Register or expose the device through the backend
5. Add UI handling if needed (buttons, icons, etc.)

The UI will automatically scale if position and zone are defined correctly.

---
# Future Improvements
* Separate button styles per device type
* Add read-only device indicators (sensors)
* Animate state changes (fade / pulse)
* Add live MQTT status indicator in UI
* Improve error handling and logging