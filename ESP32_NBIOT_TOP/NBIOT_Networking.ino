char send_atcmd (const char* ATcmd, const char* expectResp, unsigned int timeout) {
    // digitalWrite(PWR, LOW);
    // delay(8);
    // digitalWrite(PWR, HIGH);
    
    atResponse = AT_RESP_NONE;
    rx_len = 0;
    comm_buff[rx_len] = 0;

    if(ATcmd != ""){
        while (Nbmod.available() > 0) Nbmod.read();
        Nbmod.println(ATcmd);
    }

    prev_time = millis();
    do {
        if (Nbmod.available() != 0) {
            atResponse = AT_RESP_ERR;

            // read buffer for answer
            comm_buff[rx_len] = Nbmod.read();

            rx_len++;
            comm_buff[rx_len] = 0;

            // check for desired response in buffer
            if (expectResp != "") {
                if (strstr((char *)comm_buff, expectResp) != NULL) {
                    atResponse = AT_RESP_OK;
                    break;
                }
            }
            else atResponse = AT_RESP_OK;
        }
    }
    while ((millis() - prev_time) < timeout);

#if (SW_DEBUG)
  if (ATcmd != "");
    Serial.println(ATcmd);
    switch(atResponse){
      case (AT_RESP_ERR):
        Serial.print("ERROR: ");
        break;
      case (AT_RESP_NONE):
        Serial.print("Timeout: ");
        break;
    }
    for(int i =0; i<rx_len; i++)
      Serial.printf("%c", comm_buff[i]);
      
    Serial.println(rx_len);
#endif
    return atResponse;
}

bool init_nbmod() {
    pinMode(PWR, OUTPUT);
    digitalWrite(PWR, HIGH);
    char respond_code;
    int i; // generic index name
    nbModStatus = false;

    // check module status. max tries = 5
    for (i = 0; i < 5; i++) {
        respond_code = send_atcmd("AT", "OK", 2000);
        if (respond_code != AT_RESP_OK) {
            delay(1000);
            continue;
        }
        else break;
    }
    if (respond_code != AT_RESP_OK) {
        Serial.println("Module unavailable");
        nbModStatus = false;
        return false;
    }

    // Report signal quality
  for (i = 0; i < 5; i++) {
    respond_code = send_atcmd("AT+CSQ", "OK", 2000);
    if(respond_code != AT_RESP_OK){
      delay(1000);
      continue;
    }
    else break;      
  }  
  if(respond_code != AT_RESP_OK){
    Serial.println("Cmd AT_CSQ error");
    return false;
  }

  int start_time = millis();
  do {
    respond_code = send_atcmd("AT+CEREG?", "CEREG: 0,1", 2000);
    if (millis() - start_time > 60000){
      Serial.println("Wating for module to establish connection...");
      send_atcmd("AT+CPOWD=1", "", 2000);
      delay(10000);
      digitalWrite(PWR, LOW);
      delay(8);
      digitalWrite(PWR, HIGH);      
      while(1);
    }
  }
  while(respond_code != AT_RESP_OK);

  if(respond_code != AT_RESP_OK){
    Serial.println("Cmd AT_CEREG error");
    return false;
  }

  return true;
  // add more command below here i forgor
}

bool init_mqtt() {
    char respond_code;
    int i; // generic index name
    nbModStatus = false;

    // connect to mqtt server
    String at_newmqtt = "AT+CMQNEW=\"" + tbServer + "\"," + "\"" + tbPort + "\"," + "10000,1024";
    Serial.println(at_newmqtt);
    for (i = 0; i < 5; i++) {
      respond_code = send_atcmd(at_newmqtt, "+CMQNEW:0", 2000);
      if (respond_code != AT_RESP_OK) {
          delay(1000);
          continue;
      }
      else break;
    }
    if (respond_code != AT_RESP_OK) {
      Serial.println("Failed to connect to Thingsboard!");
      return false;
    }  

    String at_conmqtt = "AT+CMQCON=0,3,\"espnbclient\",600,1,0\"" + TB_DEVICE_KEY + "\",NULL";
    Serial.println(at_conmqtt);
    for (i = 0; i < 5; i++) {
      respond_code = send_atcmd(at_conmqtt, "OK", 2000);
      if (respond_code != AT_RESP_OK) {
          delay(1000);
          continue;
      }
      else break;
    }
    if (respond_code != AT_RESP_OK) {
      Serial.println("Failed to establish MQTT connection!");
      return false;
    }

    return true;
}

void send_data_to_tb(char* topic, int time_out){
  // check for connection
  respond_code = send_atcmd("AT+CMQCOM?", tbServer.toCharArray(), 2000);
  if (respond_code != AT_RESP_OK) {
        delay(1000);
        init_mqtt();
    }

  // send
  String at_pubmsg = "AT+CMQPUB=0,\"" + String(topic) + "\",1,0,0," + data_payload.length().toString + "," + data_payload;
  Serial.println(at_pubmsg);
  for (i = 0; i < 5; i++) {
    respond_code = send_atcmd(at_pubmsg, "OK", 2000);
    if (respond_code != AT_RESP_OK) {
        delay(1000);
        continue;
    }
    else break;
  }
  at_pubmsg = "";
  if (respond_code != AT_RESP_OK) {
    Serial.println("Failed to send msg!");
  }
  else 
    Serial.println("Msg sent!");

}