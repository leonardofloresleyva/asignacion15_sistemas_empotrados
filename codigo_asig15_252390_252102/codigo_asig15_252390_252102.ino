
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

int contadorFlash = 0;
bool on_off = false;

void setup() {
  
  for(int i = 0; i < 4; i++){pinMode(leds[i], OUTPUT);}

  edoLeds = APAGADO;

  Serial.begin(BAUD_RATE);

  apagarLeds();
}

void loop() {
  if(edoLeds == ERROR){
    error();
  } else{

  }
}

void apagarLeds(){
  
  for(int i = 0; i < 4; i++){digitalWrite(leds[i], LOW);}

  edoLeds = APAGADO;
}

void izquierdaDerecha(){
  if(PAUSA_LTR_RTR.update()){
    apagarLeds();
    digitalWrite(leds[contadorLtr], HIGH);
    contadorLtr++;
    if (contadorLtr > 3){contadorLtr = 0;}
  }
}

void derechaIzquierda(){
  if(PAUSA_LTR_RTR.update()){
    apagarLeds();
    digitalWrite(leds[contadorRtr], HIGH);
    contadorRtr--;
    if (contadorRtr < 0){contadorRtr = 3;}
  }
}

void error(){
  
  if(PAUSA_ERROR.update()){

    if(!on_off){
      for(int i = 0; i < 4; i++){digitalWrite(leds[i], HIGH);}
      contadorFlash++;
    } else{
      apagarLeds();
    }

    on_off = !on_off;

    if(contadorFlash == 10){
      contadorFlash = 0;
      edoLeds = APAGADO;
      on_off = false;
    }
  }
}

