**EPANET integration (current implementation)**

This document describes how the current EPANET bridge works. It reflects the implementation in [epanet/app/epanet.py](epanet/app/epanet.py).

**Overview:**

- **Purpose:** run an EPANET hydraulic simulation and bridge selected node/link data to/from PLCs over Modbus TCP, and publish snapshots to MQTT.
- **Main loop:** read PLC coils (statuses), apply them to the EPANET model, advance hydraulic step, then read node/link results and write selected values back to PLC registers and MQTT.

**Creating an EPANET water distribution network**

The water distribution network with all devices and settings is created with the EPANET 2.2. program (downloaded from the EPANET website), running only in Windows. Save the network-file as "scenario.inp" as this is the hard-coded name for the water distribution network used in HydroSim (see also below).
<img width="1152" height="603" alt="epanet_network" src="https://github.com/user-attachments/assets/cba35062-4e89-491b-8e50-6565390080be" />


**High-level flow:**

- Initialize `epyt.epanet` instance using `scenario.inp` in the `epanet/app` folder.
- Create Modbus clients for configured zones (`z0`..`z4`).
- Initialize hydraulic analysis (set duration and hydraulic time step).
- Loop:
  - Read coil/status data from each PLC (via `read_plc`).
  - Apply status/control values to EPANET (via `set_nodedata` / `set_linkdata`).
  - Run hydraulic analysis step (`runHydraulicAnalysis`, `nextHydraulicAnalysisStep`).
  - Collect node/link telemetry (`get_nodedata`, `get_linkdata`).
  - Write telemetry back to PLCs (`write_plc`).
  - Publish telemetry to MQTT topic (if configured).

**Configuration & environment:**

- File: [epanet/app/epanet.py](epanet/app/epanet.py)
- Environment variables (used by script):
  - `MQTT_BROKER_URL` – MQTT broker URL.
  - `MQTT_TOPIC` – MQTT topic to publish node/link snapshots.
  - `MQTT_CA_CERT`, `MQTT_CLIENT_CERT`, `MQTT_CLIENT_KEY`, `MQTT_TLS_ENABLED` – TLS options for MQTT.
  - `DEBUG` – when true uses local test ports for Modbus clients.
  - `LOCALHOST` – when true uses hostname `localhost` instead of `127.0.0.1` for debug clients.
  - `PRINTING` – enable extra console logging of PLC register actions.
  - `LOG_FILE`, `LOG_LEVEL` – log file and level.

By default the EPANET model used is `scenario.inp` next to the script.

**Zones and Modbus clients**

- The code expects zone IDs `z0`, `z1`, `z2`, `z3`, `z4`.
- In normal (non-DEBUG) mode it will attempt to connect to hosts named `plc-zoneX` on port 502. In DEBUG mode it connects to `127.0.0.1:5022..5026` (or `localhost` when `LOCALHOST` is set).
- Clients are created by `setup_clients(zones: list[str]) -> dict[str, ModbusTcpClient]` and retried until connection succeeds.

**Modbus mapping constants**

- `PUMP_MAPPING` and `VALVE_MAPPING` map PLC host identifiers to EPANET link indexes used to correlate coils/register indices with model elements. These mappings are used to resolve which coil/register corresponds to which pump/valve in the model.

**Important addresses and conventions in `write_plc` / `read_plc`:**

- Coils (discrete on/off) are read using `read_coils(address=0, count=32)`. The coil index mapped via `PUMP_MAPPING`/`VALVE_MAPPING` determines pump/valve `status` (1.0 or 0.0).
- Flow meter values are written to register address `700` as a FLOAT32 encoded in two 16-bit registers (big-endian word order used by `float_to_registers`).
- Tank levels are written to registers starting at address `10 + (tank_num * 2)` as FLOAT32 (two registers).
- A per-zone `sensor_mask` (bit flags) is written to holding register `0` as a single integer representing tank low/high alarm bits.

- When writing flow values the implementation sets `flow` to `0.0` if the downstream node's pressure is negative. EPANET can still compute flows for links that are effectively disconnected (for example across a closed valve) which may produce non-physical or spurious flow/pressure results; zeroing flows for negative-pressure nodes prevents reporting misleading values to PLCs/SCADA. See the EPANET documentation (PDF) discussion on disconnected components (page 99) for background.

Note: the code uses `struct.pack('>f')`/`struct.unpack('>HH')` via `float_to_registers` to produce two 16-bit registers per float (big-endian words). This matches the current PLC expectations in this deployment.

**Key functions (brief)**

- `setup_clients(zones: list[str]) -> dict[str, ModbusTcpClient]`
  - Create ModbusTcpClient per zone, connect, return dict.

- `read_plc(zone: str, client: ModbusTcpClient) -> (nodes, links)`
  - Returns two dicts keyed by EPANET `name_id` for nodes and links present in the zone.
  - Reads coils to determine discrete `status` for pumps/valves using the mapping tables.

- `set_nodedata(nodes: dict[str, dict])`
  - Apply node-side changes to EPANET (currently minimal: placeholder for future base demand/quality changes). Called after reading PLC data.

- `set_linkdata(links: dict[str, dict])`
  - Apply link-side changes to EPANET. For pumps the code uses the `status` bit (on/off) to set the pump runtime via `ep.setLinkSettings(index, new_status)`; other link types currently pass.

- `get_zone_items(zone_id: str) -> (nodes, links)`
  - Returns lists of EPANET node and link indexes that begin with the zone prefix (name startswith `zone_id`).

- `get_nodedata(nodes) -> dict`
  - Builds a dictionary of telemetry for each node: `index`, `type`, `pressure`, `quality`, `elevation`. For tanks computes `level` from tank volume/diameter.

- `get_linkdata(links) -> dict`
  - Builds link telemetry: `index`, `type`, `flow`, `headloss`, `status`, and type-specific fields (`velocity`, `length`, pump-specific `speed`, `power`, `energy`, `efficiency`, `state`).

- `float_to_registers(value) -> list[int]`
  - Packs float to two 16-bit register words (big-endian) for Modbus writes.

- `write_plc(client, nodes_data, links_data) -> None`
  - Writes flow (address 700), tank levels (addresses 10+...), sensor mask (address 0) and logs status for pumps/valves.

**Data shapes**

- `nodes_data` produced by `get_nodedata` is a mapping: { "zX-element": { "index": int, "type": "JUNCTION|TANK|RESERVOIR", "pressure": float, ... } }
- `links_data` produced by `get_linkdata` is a mapping: { "zX-element": { "index": int, "type": "PIPE|PUMP|VALVE", "flow": float, "status": 0|1, ... } }

Example (node snippet):

```
"z0-junction2": {"index": 1, "type": "JUNCTION", "pressure": 50.0, "demand": 0.123}
```

Example (link snippet):

```
"z3-pump1": {"index": 141, "type": "PUMP", "flow": 0.02, "status": 1, "speed": 1.0}
```

**MQTT publishing**

- If `MQTT_BROKER_URL` and `MQTT_TOPIC` are configured the script will connect and publish stringified dicts for node and link snapshots after each hydraulic step.

**Timing and hydraulic steps**

- The script sets simulation duration initially (`24*3600` seconds by default) and the hydraulic step (`tstep = 5*60` seconds). In the main loop the duration is incremented by the hydraulic step to effectively run indefinitely until the model completes or the user interrupts.

**Logging and debug options**

- `DEBUG` and `LOCALHOST` change the way Modbus clients are created (local ports vs PLC hostnames).
- Logs are written to `LOG_FILE` at `LOG_LEVEL`.

**Run**

- Start the script (the repository typically runs it as part of the `epanet` service). There are no CLI arguments in the current version — configuration comes from environment variables and `scenario.inp` used by `epyt`.

**Notes & considerations**

- The code currently uses coil mapping tables plus coil reads to drive pump/valve on/off state; numeric control values (e.g. pump speed as a float written from PLC) are not used — the implementation maps status bits into `ep.setLinkSettings` for pumps.
- Float encoding uses big-endian word order (see `float_to_registers`). Confirm this against your PLC/SCADA expectations before changing.
- `PUMP_MAPPING` / `VALVE_MAPPING` are hand-maintained maps of host -> element index; update them when you change the EPANET model element indexes or zone hostnames.
- The EPANET API (`epyt.epanet`) methods are used extensively — changes in `epyt` behavior may require doc updates.

---

Generated to reflect [epanet/app/epanet.py](epanet/app/epanet.py).





