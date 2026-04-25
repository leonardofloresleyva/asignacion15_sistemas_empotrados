
#include <NoDelay.h>

const int PIN_LED_0 = 2;
const int PIN_LED_1 = 4;
const int PIN_LED_2 = 18;
const int PIN_LED_3 = 19;

const int PAUSA_ERROR = 250;
const int PAUSA_LTR_RTR = 500;

// Velocidad de transmisión del puerto serie
const unsigned int BAUD_RATE = 115200;

typedef enum{
  APAGADO,
  LTR,
  RTR,
  ERROR
} estadoLeds;

estadoLeds edoLeds;

noDelay pausaError(PAUSA_ERROR);
noDelay pausaLtrRtr(PAUSA_LTR_RTR);

void apagarLeds();
void izquierdaDerecha();
void derechaIzquierda();
void error();

void setup() {

  pinMode(PIN_LED_0, OUTPUT);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);

  edoLeds = APAGADO;

  Serial.begin(BAUD_RATE);

  apagarLeds();
}

void loop() {
  

}

void apagarLeds(){
  
  digitalWrite(PIN_LED_0, LOW);
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  digitalWrite(PIN_LED_3, LOW);

  edoLeds = APAGADO;
}

