#!/bin/bash
# =====================================================
# Script: generate_ca.sh
# Goal: Generates a CA private key and rootcertificate
# =====================================================

# Stop in case of errors
set -e

# Ask for a Common Name
read -p "Enter the Common Name (CN) for the CA (e.g. MyCA): " CN

# Check if CN isn't empty
if [ -z "$CN" ]; then
  echo "Error: Common Name can not be empty."
  exit 1
fi

# Paths (change if necessary)
BASE_DIR="./certs"
CA_DIR="$BASE_DIR/ca"

# Create the CA directory if it doesn't exist
if [ ! -d "$CA_DIR" ]; then
  echo "CA directory doesn't exist yet. Creating $CA_DIR..."
  mkdir -p "$CA_DIR" || {
    echo "Error: Couldn't create directory $CA_DIR" >&2
    exit 1
  }
fi

# Filepaths
CA_KEY="$CA_DIR/ca.key"
CA_CERT="$CA_DIR/ca.crt"

echo "-------------------------------------------"
echo "Generating CA private key..."
echo "-------------------------------------------"

openssl genrsa -out "$CA_KEY" 4096

# Set the permissions of the private key so the mosquitto container can read the file
chmod 644 "$CA_KEY"

echo "-------------------------------------------"
echo "Creating the CA rootcertificate (valid for 10 years)..."
echo "-------------------------------------------"

# Creating the rootcertificate (change -subj parameters if necessary)
openssl req -new -x509 -days 3650 -key "$CA_KEY" -out "$CA_CERT" -subj "/C=NL/O=HHS/CN=${CN}"

echo "-------------------------------------------"
echo "CA certificate succesfully generated!"
echo "Files:"
echo "  Private key : $CA_KEY"
echo "  Certificate : $CA_CERT"
echo "-------------------------------------------"
