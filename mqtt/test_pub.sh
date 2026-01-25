#!/bin/bash

#Change --host to actual IP address of the broker 
mosquitto_pub \
  --host 192.168.2.55 \
  --port 8883 \
  --cafile certs/ca/ca.crt \
  --cert certs/client/Test_Pub.crt \
  --key certs/client/Test_Pub.key \
  --topic "test" \
  --message "Testing MQTT with TLS encryption and certificate authentication!"
