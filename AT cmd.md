# init SIM module (in order)
- AT -> respond OK
- AT+CSQ -> signal quality report -> 0,0
- 
- 

# MQTT
- Define TOKEN of device -> username, null -> password
- host: demo.thingsboard.io : 1883
- pub topic: v1/devices/me/telemetry
- New MQTT: AT+CMQNEW=<sv>,<port>,<command_timeout>,<bufsize> => mqtt_id
    ```
    AT+CMQNEW="demo.thingsboard.io","1883",10000,1024
    ```

- Connect MQTT: AT+CMQCON=<mqtt_id>,<version(3,4)>,<client_id>,<keep_alive_interval>,<cleansesession>,<will_flag>[,<will_options>][,<user>,<pass>]
    ```
    AT+CMQCON=0,3,"espnbclient",600,1,0
    ```

- Subscribe: AT+CMQSUB=<mqtt_id>,<topic>,<QoS>
    ```
    AT+CMQSUB=0,"mytopic",0
    ```

- Publish: AT+CMQPUB=<mqtt_id>,<topic>,<QoS>,<retained>,<dup>,<msg_len>,<msg>
  msg default type is hex: AT+CREVHEX=1
  to set raw use: AT+CREVHEX=0 => json
    ```
    AT+CMQPUB=0,"mytopic",1,0,0,8,"31323334"
    ```

- Disconnect: AT+CMQDISCON=<mqtt_id>


