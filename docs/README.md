# HydroSim Documentation

## Simulation
- `openplc.md` - OpenPLC documentation for the simulation
- `scadalts.md` - SCADA Human Machine Interface with realistisc controls for the simulation
- `epanet.md` - EPANET simulation with Modbus controls
- `opensearch.md` - OpenSearch Dashboard for logging and monitoring
- `webserver.md` - The Apache webserver serving as the initial attack surface
- `mqtt.md` - The MQTT broker, used to send the HydroSim simulation state to physical hardware.

## Physical hardware
Checkout the `Physical hardware` branch in this GitHub.

## Vulnerabilities
 - `vulnerabilities.md` - Overview of each HydroSim component and its vulnerabilities with CVE code and description
 - `vulnerability_modbus.md` - Proof of Concept vulnerability for overwriting OpenPLC registers via Modbus communication
 