#ifndef RIEGO_H
#define RIEGO_H

#include <Arduino.h>
#include <TimerOne.h>
#include "Bomba.h"
#include "Valvula.h"

void parpadeoLED3(); // callback global TimerOne

class Riego {
public:
    Riego(Bomba& bomba, Valvula& valvula, int pinLED3);
    void iniciar(int caso);
    void verificarDuracion();
    void finalizar();

    bool activo;
    bool fin;
    int  tipo;

private:
    Bomba&    _bomba;
    Valvula&  _valvula;
    int       _pinLED3;
    unsigned long _inicioRiego;
};

#endif