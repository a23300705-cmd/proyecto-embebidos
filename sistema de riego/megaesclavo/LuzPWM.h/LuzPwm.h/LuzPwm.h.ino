#ifndef LuzPWM_H
#define LuzPWM_H

#include <Arduino.h>
#include <TimerOne.h>

class LuzPWM {
public:
    LuzPWM(int pin);
    void begin();
    void actualizar(int valorADC); // recibe 0-1023 directo

private:
    int _pin;
};

#endif