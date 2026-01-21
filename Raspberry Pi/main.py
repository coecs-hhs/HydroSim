#!/usr/bin/env python3
"""
Main script for reading JSON data or MQTT messages and writing to Modbus registers.

Supports:
- Offline mode (JSON file)
- Live mode via MQTT with TLS security

Dependencies:
- pymodbus
- paho.mqtt.client
- dotenv
- dataklasse.py (data classes)
"""

import json
import ast
import ssl
import os
import time
import logging
from pathlib import Path

from pymodbus.client import ModbusTcpClient
from dotenv import load_dotenv
import paho.mqtt.client as mqtt

from dataklasse import create_objects as dk

# ---------------- Logging configuration ----------------
logging.basicConfig(level=logging.INFO)
logging.getLogger("pymodbus").setLevel(logging.INFO)

# ---------------- Load environment variables ----------------
load_dotenv()
base_path = Path(__file__).parent.resolve()

# ---------------- MQTT / TLS SETTINGS ----------------
MQTT_ENABLED = True
MQTT_BROKER = os.getenv("MQTT_BROKER_URL")
MQTT_PORT = int(os.getenv("MQTT_PORT", 1883))
MQTT_TOPIC = os.getenv("MQTT_TOPIC")
client = None

# TLS certificates
CA_CERT = Path(base_path / os.getenv("MQTT_CA_CERT", ""))
CLIENT_CERT = Path(base_path / os.getenv("MQTT_CLIENT_CERT", ""))
CLIENT_KEY = Path(base_path / os.getenv("MQTT_CLIENT_KEY", ""))

# ---------------- MODBUS SETTINGS ---------------------
MODBUS_IP = os.getenv("MODBUS_IP", "127.0.0.1")
MODBUS_PORT = int(os.getenv("MODBUS_PORT", 502))

modbus = ModbusTcpClient(MODBUS_IP, port=MODBUS_PORT, retries=3)
time.sleep(1)


# ---------------- FUNCTIONS ---------------------

def wait_for_flag(address: int):
    """
    Blocks until the 'Ready Flag' at the given register is 0.

    Parameters:
        address (int): Start address of the register containing the Ready Flag
    """
    while True:
        result = modbus.read_holding_registers(address + 18, count=1)
        ready_value = result.registers[0]
        if ready_value == 0:
            break
        time.sleep(0.1)


def on_connect(client, userdata, flags, rc, properties):
    """
    MQTT connection callback.

    Parameters:
        client: MQTT client instance
        userdata: user data (not used)
        flags: connect flags
        rc: result code
        properties: optional extra properties
    """
    logging.debug("MQTT connected")
    client.subscribe(MQTT_TOPIC)
    logging.info("Ready to send")


def on_message(client, userdata, msg, properties=None):
    """
    MQTT message callback.
    Parses JSON payload, creates objects, and writes attributes to Modbus.

    Parameters:
        client: MQTT client instance
        userdata: user data (not used)
        msg: MQTT message
        properties: optional extra properties
    """
    payload = msg.payload.decode("utf-8")

    try:
        payload_dict = ast.literal_eval(payload)
        parsed_data = dk(payload_dict)
        

        for parsed in parsed_data:
            address = parsed.register
            wait_for_flag(address)
            
            logging.debug(f"--- {parsed.__class__.__name__} ---")
            
            attributes = parsed.attributes()
            modbus.write_registers(address, attributes)
            modbus.write_register(address + 18, 1)

    except Exception as e:
        logging.error("Error:", exc_info=e)


# ---------------- MQTT CLIENT SETUP ---------------------
if MQTT_ENABLED:
    try:
        client = mqtt.Client(client_id="RPI", protocol=mqtt.MQTTv311, callback_api_version=mqtt.CallbackAPIVersion.VERSION2)

        # TLS configuration
        if CA_CERT.exists() and CLIENT_CERT.exists() and CLIENT_KEY.exists():
            client.tls_set(
                ca_certs=CA_CERT,
                certfile=CLIENT_CERT,
                keyfile=CLIENT_KEY,
                tls_version=ssl.PROTOCOL_TLSv1_2
            )
        else:
            raise FileNotFoundError("One or more TLS certificates are missing")

        client.tls_insecure_set(True)  # Do not strictly verify server certificates

        # Set MQTT callbacks
        client.on_connect = on_connect
        client.on_message = on_message

        # Connect and start MQTT loop
        logging.debug("Connecting to MQTT via TLS...")
        client.connect(MQTT_BROKER, MQTT_PORT)
        time.sleep(2)
        client.loop_forever()

    except KeyboardInterrupt:
        logging.info("Stopping...")
    except Exception as e:
        logging.error(f"Cannot connect to MQTT broker: {e}")


# ---------------- Offline JSON mode ---------------------
if not MQTT_ENABLED:
    try:
        with open("newdata.json", "r", encoding="utf-8") as f:
            data = json.load(f)

        parsed_data = dk(data)
        for parsed in parsed_data:
            address = parsed.register
            wait_for_flag(address)
            logging.info(f"--- {parsed.__class__.__name__} ---")
            attributes = parsed.attributes()
            modbus.write_registers(address, attributes)
            modbus.write_register(address + 18, 1)

    except KeyboardInterrupt:
        logging.info("Stopping...")
    except Exception as e:
        logging.error(f"Errors: {e}")

# Close Modbus connection
modbus.close()
