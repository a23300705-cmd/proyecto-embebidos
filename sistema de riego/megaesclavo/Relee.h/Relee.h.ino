#ifndef RELEE_H
#define RELEE_H

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