#include <TimerOne.h>
#include "Sensor.h"
#include "LuzPWM.h"
#include "Relee.h"

Sensor      sensor(A0);
LuzPWM      luz(11);
Relee valvula(7);

void setup() {
    Serial.begin(9600);
    sensor.begin();
    luz.begin();
    valvula.begin();
}

void loop() {
    int raw      = sensor.leerValorRaw();
    int humedad  = sensor.leerHumedad();

    luz.actualizar(raw);

    if (Serial.available()) {
        char comando = Serial.read();

        if (comando == 'H') {
            Serial.println(humedad);
        }

        if (comando == 'V') {
            unsigned long t = millis();
            while (!Serial.available() && millis() - t < 100);

            if (Serial.available()) {
                char estado = Serial.read();
                if (estado == '1')      valvula.abrir();
                else if (estado == '0') valvula.cerrar();
            }
        }
    }
}