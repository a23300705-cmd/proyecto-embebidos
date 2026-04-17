#include <Wire.h>
#include "Rele.h"

Rele bomba(7);

void setup() {
    bomba.begin();
    Wire.begin(8);
    Wire.onReceive(recibirComando);
}

void loop() {}

void recibirComando(int bytes) {
    char comando = Wire.read();

    if (comando == '1')     bomba.encender();
    else if (comando == '0') bomba.apagar();
}