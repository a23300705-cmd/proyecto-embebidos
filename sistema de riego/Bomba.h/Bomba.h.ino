#ifndef BOMBA_H
#define BOMBA_H

#include <Wire.h>

class Bomba {
public:
    Bomba(int direccionI2C);
    void encender();
    void apagar();

private:
    int _direccion;
};

#endif