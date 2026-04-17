#ifndef LUZPWM_H
#define LUZPWM_H

#include <TimerOne.h>

class LuzPWM {
public:
    LuzPWM(int pin);
    void begin();
    void actualizar(int valorRaw);

private:
    int _pin;
};

#endif