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

  for (i = 0; i < 5; i++) {
    respond_code = send_atcmd("AT+CEREG?", "CEREG:", 2000);
    if(respond_code != AT_RESP_OK){
      delay(1000);
      continue;
    }
    else if (strstr((char*) comm_buff, "0,2")){
      delay(1000);
      continue;
    }
    else
      break;      
  }  
  if(respond_code != AT_RESP_OK){
    Serial.println("Cmd AT_CEREG error");
    return false;
  }

  // add more command below here i forgor
}

void init_mqtt() {
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
        // return false;
    }
    else nbModStatus = true;
}

