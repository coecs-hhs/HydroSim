#!/bin/bash
set -e

./generate_ca.sh

./generate_server_certs.sh

read -p "How many clients do you need?: " n

# validation: only numbers greater than 0.
if ! [[ "$n" =~ ^[1-9][0-9]*$ ]]; then
  echo "Error: enter a number greater than 0." >&2
  exit 1
fi

for (( i=1; i<=n; i++ )); do
  ./generate_client_certs.sh
done
