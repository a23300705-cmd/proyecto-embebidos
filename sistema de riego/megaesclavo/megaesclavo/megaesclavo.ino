#include <SPI.h>
#include <TimerOne.h>
#include "Sensor.h"
#include "LuzPWM.h"
#include "Relee.h"

Sensor sensor(A0);
IndicadorPWM  indicador(11);
Relee   valvula(7);

volatile bool datoListo  = false;
volatile char comando    = 0;
volatile char subcomando = 0;

int humedad  = 0;
int valorADC = 0;

ISR(SPI_STC_vect) {
    char recibido = SPDR;

    if (recibido == 'H') {
        SPDR    = (uint8_t)humedad;
        comando = 0;
    }
    else if (recibido == 'V') {
        comando = 'V';
        SPDR    = 0x00;
    }
    else if (comando == 'V') {
        subcomando = recibido;
        datoListo  = true;
        comando    = 0;
        SPDR       = 0x00;
    }
}

void setup() {
    sensor.begin();
    indicador.begin();
    valvula.begin();

    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);
    SPCR &= ~_BV(MSTR);
    SPCR |= _BV(SPIE);
    sei();
}

void loop() {
    valorADC = analogRead(A0);        // raw para PWM
    humedad  = sensor.leerHumedad();  // 0-100 para SPI
    indicador.actualizar(valorADC);   // brillo invertido

    if (datoListo) {
        datoListo = false;
        if (subcomando == '1')      valvula.abrir();
        else if (subcomando == '0') valvula.cerrar();
    }
}