# init SIM module (in order)
- AT -> respond OK
- AT+CSQ -> signal quality report

# MQTT
- Define TOKEN of device -> username, null -> password
- host: demo.thingsboard.io : 1883
- pub topic: v1/devices/me/telemetry