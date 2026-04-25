
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

int leds[] = {PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3};

int contadorLtr = 0;
int contadorRtr = 3;

void setup() {
  
  for(int i = 0; i < 4; i++){pinMode(leds[i], OUTPUT);}

  edoLeds = APAGADO;

  Serial.begin(BAUD_RATE);

  apagarLeds();
}

void loop() {


}

void apagarLeds(){
  
  for(int i = 0; i < 4; i++){digitalWrite(leds[i], LOW);}

  edoLeds = APAGADO;
}

void izquierdaDerecha(){
  
  if(PAUSA_LTR_RTR.update()){
    
    digitalWrite(leds[contadorLtr], LOW);
    contador++;
    if(contadorLtr > 3){contadorLtr = 0;}
    digitalWrite(leds[contadorLtr], HIGH);
  }
}
void derechaIzquierda(){
  if(PAUSA_LTR_RTR.update()){

  }
}
void error(){

}

