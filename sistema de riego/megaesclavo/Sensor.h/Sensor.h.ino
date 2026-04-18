#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
public:
    Sensor(int pin);
    void begin();
    int  leerHumedad(); // retorna 0-100

private:
    int _pin;
};

#endif