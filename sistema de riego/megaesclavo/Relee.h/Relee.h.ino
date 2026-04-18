#ifndef RELEE_H
#define RELEE_H

#include <Arduino.h>

class Relee {
public:
    Relee(int pin);
    void begin();
    void abrir();
    void cerrar();

private:
    int _pin;
};

#endif