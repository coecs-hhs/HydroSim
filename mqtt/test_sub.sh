#!/bin/bash

#Change --host to actual IP address of the broker 
mosquitto_sub \
  --host 192.168.2.55 \
  --port 8883 \
  --cafile certs/ca/ca.crt \
  --cert certs/client/Test_Sub.crt \
  --key certs/client/Test_Sub.key \
  --topic "test" \
  --verbose
