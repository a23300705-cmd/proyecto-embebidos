#ifndef RELE_H
#define RELE_H

#include <Arduino.h>

class Rele {
public:
    Rele(int pin);
    void begin();
    void encender();
    void apagar();

private:
    int _pin;
};

#endif