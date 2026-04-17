#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
public:
    Sensor(int pinPot);
    void begin();
    int leerHumedad();
    int leerValorRaw();

private:
    int _pin;
};

#endif