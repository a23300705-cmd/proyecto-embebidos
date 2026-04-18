//Este bloque es del MASTER (Mega)
#include <Wire.h>

const int direccionUNO = 8;
int humedad = 0; // viene del sistema, aquí simulado

void setup() {
    Serial.begin(9600);
    Wire.begin();
}

void encenderBomba() {
    Wire.beginTransmission(direccionUNO);
    Wire.write('1');
    Wire.endTransmission();
}

void apagarBomba() {
    Wire.beginTransmission(direccionUNO);
    Wire.write('0');
    Wire.endTransmission();
}

void loop() {
    // ejemplo: enciende si humedad <= 66%
    if (humedad <= 66) {
        encenderBomba();
        Serial.println("Bomba: ON");
    } else {
        apagarBomba();
        Serial.println("Bomba: OFF");
    }

    delay(1000);
}


//Este bloque es del ESCLAVO (UNO)
#include <Wire.h>

const int rele = 7;

void setup() {
    pinMode(rele, OUTPUT);
    digitalWrite(rele, LOW);

    Wire.begin(8);
    Wire.onReceive(recibirComando);
}

void loop() {}

void recibirComando(int bytes) {
    char comando = Wire.read();
    if (comando == '1')      digitalWrite(rele, HIGH);
    else if (comando == '0') digitalWrite(rele, LOW);
}