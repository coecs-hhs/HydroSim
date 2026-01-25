#!/bin/bash
# =====================================================
# Script: generate_client_certs.sh
# Doel: Generates a client key and certificate for Mosquitto TLS
# =====================================================

# Stop in case of errors
set -e

# Ask for the Common Name of the client
read -p "Enter the Common Name (CN) (e.g. client1, HydroSim or RPI): " CN

# Check if CN isn't empty
if [ -z "$CN" ]; then
  echo "Error: Common Name can not be empty."
  exit 1
fi

# Paths (change if necessary)
BASE_DIR="./certs"
CLIENT_DIR="$BASE_DIR/client"
CA_DIR="$BASE_DIR/ca"

# Create the client directory if it doesn't exist
if [ ! -d "$CLIENT_DIR" ]; then
  echo "Client directory doesn't exist yet. Creating $CLIENT_DIR..."
  mkdir -p "$CLIENT_DIR" || {
    echo "Error: Couldn't create directory $CLIENT_DIR" >&2
    exit 1
  }
fi

# Filepaths
KEY_FILE="$CLIENT_DIR/${CN}.key"
CSR_FILE="$CLIENT_DIR/${CN}.csr"
CRT_FILE="$CLIENT_DIR/${CN}.crt"

echo "-------------------------------------------"
echo "Generating the private key for: $CN"
echo "-------------------------------------------"

openssl genrsa -out "$KEY_FILE" 4096

# Set the permissions of the client key so the mosquitto container can read the file
chmod 644 "$KEY_FILE"

echo "-------------------------------------------"
echo "Generating the server CSR..."
echo "-------------------------------------------"

# Creating the client CSR (change -subj parameters if necessary)
openssl req -new -key "$KEY_FILE" -out "$CSR_FILE" -subj "/C=NL/O=HHS/CN=${CN}"

echo "-------------------------------------------"
echo "Signing with CA (valid for 1 year)..."
echo "-------------------------------------------"

openssl x509 -req -in "$CSR_FILE" \
  -CA "$CA_DIR/ca.crt" \
  -CAkey "$CA_DIR/ca.key" \
  -CAserial "$CA_DIR/ca.srl" \
  -out "$CRT_FILE" \
  -days 365

echo "-------------------------------------------"
echo "Client certificate succesfully generated!"
echo "Files:"
echo "  Private key : $KEY_FILE"
echo "  Certificate : $CRT_FILE"
echo "  CSR         : $CSR_FILE"
echo "-------------------------------------------"
