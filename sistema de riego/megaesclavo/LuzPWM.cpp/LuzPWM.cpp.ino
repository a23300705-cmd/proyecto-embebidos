#include "LuzPWM.h"

LuzPWM::LuzPWM(int pin) {
    _pin = pin;
}

void LuzPWM::begin() {
    Timer1.initialize(1000);
    Timer1.pwm(_pin, 0);
}

void LuzPWM::actualizar(int valorRaw) {
    Timer1.setPwmDuty(_pin, 1023 - valorRaw);
}