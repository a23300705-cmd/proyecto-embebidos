#ifndef BOMBA_H
#define BOMBA_H

#include <Arduino.h>
#include <Wire.h>

class Bomba {
public:
    Bomba(int direccionI2C);
    void begin();
    void encender();
    void apagar();

private:
    int _direccion;
};

#endif