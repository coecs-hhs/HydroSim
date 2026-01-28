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











## Future improvements

- **Filtering by zone**
  - You could easily filter by zone using two methods:

    1. **Filter in the `create_objects` function in `dataklasse.py`:**
       ```python
       def create_objects(json_string: dict):
           if not isinstance(json_string, dict):
               raise ValueError("JSON must be a dictionary of objects")

           objects = []
           pattern = r'^([a-zA-Z]+)(\d+)-(pipe|pump|valve|house|junction|tank)(\d+)$'

           for outer_key, inner_dict in json_string.items():
               match = re.search(pattern, outer_key)

               if match is None or (match.group(3) == "house" and int(match.group(4)) > 4):
                   continue

               # Add filter here, for example:
               if int(match.group(2)) == 2:
                   pass  # filter on zone 2

               obj = create_object_from_data(inner_dict)
               if obj is None:
                   continue

               obj.zone = int(match.group(2))
               obj.id = int(match.group(4))
               objects.append(obj)

           return objects
       ```

    2. **Filter using MQTT topics**
       - Currently, the wildcard `#` is used, which means all available topics are subscribed to.
       - This would require changes on the server side so it publishes to separate topics.
       - This would allow subscribing only to a specific zone, for example `"zone_1"`.

- **MQTT reconnect after disconnect**
  - The easiest solution would be to place the MQTT connect function inside a loop so it retries the connection.
  - Alternatively, callback methods might be available to accomplish this more cleanly.

- **MODBUS reconnect after disconnect**
  - Similar to MQTT, the connect function could be placed in a loop so it automatically retries when the connection is lost.


- **Simulation <-> hardware parity & bidirectional communication**
  - Currently, the simulation and the hardware implementation mainly focus on receiving data.
    To improve reliability and maintainability, the simulation and hardware should mirror each other more closely in terms of interfaces and behavior.

  - A future improvement would be to ensure that both the simulation and the hardware:
    - Use the same data formats and message structures
    - Share a common interface for MQTT and MODBUS communication
    - React to incoming data in the same way

  - Additionally, the hardware could be extended to support **bidirectional communication**.
    At the moment, the hardware primarily acts as a receiver, but it could also:
    - Send status updates or acknowledgements
    - Publish sensor states or diagnostics
    - Report errors or connection states back to the server

  - This would allow the simulation to better represent real hardware behavior and enable more advanced testing scenarios before deploying to physical devices.

