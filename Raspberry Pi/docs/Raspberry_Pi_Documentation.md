# HydroSim Raspberry PI (Ubuntu) Network Modbus-MQTT Interface

## Overview
This project reads JSON data or MQTT messages representing water network objects
(e.g., Pump, Valve, Tank, Pipe) and writes their attributes to Modbus registers.

It supports:
- Offline mode (reading from JSON)
- Live mode via MQTT with TLS

Make sure the Raspberry Pi is configured according to [Netplan_Configuration.md](Netplan_Configuration.md),
and make sure OpenVPN is properly installed [OpenVPN](OpenVPN_Guide.md).

## Dependencies
- Python 3.9+ (version used: 3.13.7)
- [pymodbus](https://pypi.org/project/pymodbus/) (version used: 3.11.4)
- [paho-mqtt](https://pypi.org/project/paho-mqtt/) (version used: 2.1.0)
- [python-dotenv](https://pypi.org/project/python-dotenv/) (version used: 1.2.1)

## Configuration
Create a `.env` file with the following variables:

```
MQTT_BROKER_URL=145.52.126.130
MQTT_TOPIC=#
MQTT_PORT=8883
MQTT_TLS_ENABLED=true
MQTT_CA_CERT=./certs/ca.crt
MQTT_CLIENT_CERT=./certs/client/client.crt
MQTT_CLIENT_KEY=certs/client/client.key
MODBUS_IP=192.168.1.10
MODBUS_PORT=502
```

## Usage
### Offline mode
Set `MQTT_ENABLED = False` in `main.py`. Provide a JSON file `newdata.json` with object data.
```bash
python main.py
```

### MQTT mode
Set `MQTT_ENABLED = True`. The script connects to the broker and continuously listens for messages.

## Classes
### BaseData
- Base class for all objects
- Attributes:
  - `zone` (int)
  - `id` (int)
- Methods:
  - `attributes()`: Returns a list of values ready for Modbus

### Subclasses
| Class   | Register | Attributes |
|---------|----------|------------|
| Pump    | 0        | status, flow, speed, quality |
| Valve   | 20       | status, flow, quality |
| Junction| 40       | demand, quality, pressure |
| Tank    | 60       | level, quality, pressure |
| Pipe    | 80       | flow, quality |

## Factory Functions
- `create_object_from_data(data: dict) -> BaseData`
- `create_objects(json_string: dict) -> list[BaseData]`

## Logging
- Debug level enabled by default
- Modbus logging set to INFO

## Modbus Protocol
- Each object has a dedicated register range
- Ready Flag is located at `register + 18`
- Values are rounded and scaled before writing

