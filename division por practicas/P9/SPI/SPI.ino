// ste bloque es del MASTER (Mega)
cpp#include <SPI.h>

const int CS = 53;
int humedad  = 0;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE0);

    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
}

void pedirHumedad() {
    digitalWrite(CS, LOW);
    SPI.transfer('H');
    digitalWrite(CS, HIGH);
}

int recibirHumedad() {
    delay(10);
    digitalWrite(CS, LOW);
    int val = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return val;
}

void abrirValvula() {
    digitalWrite(CS, LOW);
    SPI.transfer('V'); SPI.transfer('1');
    digitalWrite(CS, HIGH);
}

void cerrarValvula() {
    digitalWrite(CS, LOW);
    SPI.transfer('V'); SPI.transfer('0');
    digitalWrite(CS, HIGH);
}

void loop() {
    pedirHumedad();
    humedad = recibirHumedad();

    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.print("%  |  Valvula: ");

    if (humedad <= 33) {
        abrirValvula();
        Serial.println("ABIERTA");
    } else {
        cerrarValvula();
        Serial.println("CERRADA");
    }

    delay(1000);
}
//Este bloque es del ESCLAVO (Mega esclavo)
#include <TimerOne.h>

const int pot    = A0;
const int ledPWM = 11;
const int releValvula = 7;

volatile int  humedad    = 0;
volatile int  valorADC   = 0;
volatile bool datoListo  = false;
volatile char comando    = 0;
volatile char subcomando = 0;

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
    pinMode(MISO,        OUTPUT);
    pinMode(releValvula, OUTPUT);
    digitalWrite(releValvula, LOW);

    // SPI modo esclavo
    SPCR |= _BV(SPE);
    SPCR &= ~_BV(MSTR);
    SPCR |= _BV(SPIE);
    sei();

    // TimerOne PWM 10 bits
    Timer1.initialize(1000);
    Timer1.pwm(ledPWM, 0);
}

void loop() {
    valorADC = analogRead(pot);
    humedad  = map(valorADC, 0, 1023, 100, 0);

    Timer1.setPwmDuty(ledPWM, 1023 - valorADC);

    if (datoListo) {
        datoListo = false;
        if (subcomando == '1')      digitalWrite(releValvula, HIGH);
        else if (subcomando == '0') digitalWrite(releValvula, LOW);
    }
}