#include "top_config.h"

void setup() {
    Serial.begin(115200);
    while(!Serial);
    Serial.println("Serial started!");
    init_system();
}

void loop () {
  COM_monitor();
  thingsboard_communication();
}

void thingsboard_communication(){
  if(tb_data_valid == false) return;
  
  if(millis() - tb_send_timming > TB_DURATION){        
    send_data_to_tb();
    tb_data_valid = false;;
    tb_send_timming = millis();
  }
}

void init_system() {
    // init sensors
    // init_sensors();

    // init nbiot module
    Serial.println("init NB-IOT");
    Nbmod.begin(115200, SERIAL_8N1, RXD, TXD);
    delay(5000);
    if (init_nbmod() != false) init_mqtt();
    send_atcmd("AT", "OK", 2000);
}

void COM_monitor(){  
  while (Nbmod.available()) {
    Serial.write(Nbmod.read());//Forward what Software Serial received to Serial Port
  }
}