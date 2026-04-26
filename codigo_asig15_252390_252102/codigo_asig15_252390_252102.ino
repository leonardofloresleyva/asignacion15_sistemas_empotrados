
#include <NoDelay.h>

const int PIN_LED_0 = 2;
const int PIN_LED_1 = 4;
const int PIN_LED_2 = 18;
const int PIN_LED_3 = 19;

const int PAUSA_ERROR = 250;
const int PAUSA_LTR_RTL = 500;

// Velocidad de transmisión del puerto serie
const unsigned int BAUD_RATE = 115200;

typedef enum{
  APAGADO,
  LTR,
  RTL,
  ERROR
} estadoLeds;

estadoLeds edoLeds;

noDelay pausaError(PAUSA_ERROR);
noDelay pausaLtrRTL(PAUSA_LTR_RTL);

void apagarLeds();
void izquierdaDerecha();
void derechaIzquierda();
void error();

int leds[] = {PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3};

int contadorLtr = 0;
int contadorRTL = 3;

int contadorFlash = 0;
bool on_off = false;

void setup() {
  
  for(int i = 0; i < 4; i++){pinMode(leds[i], OUTPUT);}

  edoLeds = APAGADO;

  Serial.begin(BAUD_RATE);

  apagarLeds();
}

void loop() {
  
  if (edoLeds != ERROR) {
    
    if (Serial.available() > 0) {
      String comando = Serial.readStringUntil('\n');
      comando.trim();

      if (comando == "off") {
        apagarLeds(); 
        edoLeds = APAGADO;
      }
      else if (comando == "ltr" && edoLeds == APAGADO) {
        edoLeds = LTR;
        contadorLtr = 0;
      } 
      else if (comando == "rtl" && edoLeds == APAGADO) {
        edoLeds = RTL;
        contadorRTL = 3;
      } 
      else {edoLeds = ERROR;}
    }
  }

  switch (edoLeds) {
    case APAGADO:
      break;
    case LTR:
      izquierdaDerecha();
      break;
    case RTL:
      derechaIzquierda();
      break;
    case ERROR:
      error();
      break;
  }
}

void apagarLeds(){for(int i = 0; i < 4; i++){digitalWrite(leds[i], LOW);}}

void izquierdaDerecha(){
  if(pausaLtrRTL.update()){
    if(contadorLtr == 0){
      digitalWrite(leds[3], LOW);
      digitalWrite(leds[contadorLtr++], HIGH);
    } else if(contadorLtr > 0 && contadorLtr <= 3){
      digitalWrite(leds[contadorLtr - 1], LOW);
      digitalWrite(leds[contadorLtr++], HIGH);
      if(contadorLtr > 3){
        contadorLtr = 0;
      }
    }
  }
}

void derechaIzquierda(){
  if(pausaLtrRTL.update()){
    if(contadorRTL == 3){
      digitalWrite(leds[0], LOW);
      digitalWrite(leds[contadorRTL--], HIGH);
    }
    else if(contadorRTL >= 0 && contadorRTL < 3){
      digitalWrite(leds[contadorRTL + 1], LOW);
      digitalWrite(leds[contadorRTL--], HIGH);
      if(contadorRTL < 0){contadorRTL = 3;}
    }
  }
}

void error(){
  
  if(pausaError.update()){

    if(contadorFlash < 10){
      
      on_off = !on_off;

      if(on_off){
        for(int i = 0; i < 4; i++){digitalWrite(leds[i], HIGH);}
        contadorFlash++;
      } else{apagarLeds();}
      
    } else{
      contadorFlash = 0;
      on_off = false;
      apagarLeds();
      edoLeds = APAGADO;
    }
  }
}