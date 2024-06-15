#include "sensors_config.h"

bool read_sensors(){
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    if (isnan(hum) || isnan(temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return false;
    }   

    // if (lightMeter.measurementReady()) {
    //     lux = lightMeter.readLightLevel();
    // }

    // if(lux > LUX_THRESHOLD){
    //     digitalWrite(LED_PIN, LOW);
    // } else digitalWrite(LED_PIN, HIGH);
    // return true;
}

void init_sensors(){			
    // Wire.begin();
    // if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    //   Serial.println(F("BH1750 init OK"));
    // } 
    // else {
    //   Serial.println(F("Error initialising BH1750"));
    // }

    dht.begin();
    delay(50);
    bool dht_en = false;
    int num_dht_err = 0;
    while(num_dht_err < 5){        
        temp = dht.readHumidity();
        hum = dht.readTemperature();    

        if (isnan(hum) || isnan(temp)) {
          delay(500);
          num_dht_err++;
          continue;
        }
        else{      
          num_dht_err = 0;  
          break;
        }  
    }

    if(num_dht_err >= 5){
        Serial.println(F("Failed to init DHT sensor!"));
        dht_en = false;
    }
    else{ 
        dht_en = true;    
        Serial.println(F("DHT init OK"));
    }

    pinMode(LED_PIN, OUTPUT);
}