#include "top_config.h"

void setup() {
    Serial.begin(115299);

    init_system();
}

void loop () {

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