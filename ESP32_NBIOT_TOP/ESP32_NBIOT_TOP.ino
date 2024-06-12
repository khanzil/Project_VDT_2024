#include "top_config.h"

float lux, temp, hum;

void setup() {
    Serial.begin(115200);
    init_system();
}

void loop () {
    read_sensors();

}

void init_system() {
    // init sensors
    init_sensors();

    // init nbiot module
    Serial.println("init NB-IOT");
    Nbmod.begin(115200, SERIAL_8N1, RXD, TXD);
    delay(5000);
    init_nbmod();

}