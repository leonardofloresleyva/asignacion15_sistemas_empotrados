// Biblioteca NoDelay para código no bloqueante
#include <NoDelay.h>

// Pines de cada led
const int PIN_LED_0 = 2;
const int PIN_LED_1 = 4;
const int PIN_LED_2 = 18;
const int PIN_LED_3 = 19;

// Tiempo de espera para cada flasheo
const int PAUSA_ERROR = 250;

// Tiempo de espera para el cambio de leds (de izquierda a derecha o viceversa)
const int PAUSA_LTR_RTL = 500;

// Velocidad de transmisión del puerto serie
const unsigned int BAUD_RATE = 115200;

// Estados del circuito
typedef enum {
  APAGADO,
  LTR,
  RTL,
  ERROR
} estadoLeds;

// Variable que almacena el estado actual del circuito
estadoLeds edoLeds;

// Instancias de la clase NoDelay para cada tiempo de espera
noDelay pausaError(PAUSA_ERROR);
noDelay pausaLtrRTL(PAUSA_LTR_RTL);

// Funciones de transición
void apagarLeds();
void izquierdaDerecha();
void derechaIzquierda();
void error();

// Arreglo que almacena los pines de cada led
int leds[] = { PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3 };

// Índices del led encendido actual para el recorrido de los leds
int contadorLtr = 0;
int contadorRtl = 3;

// Contador de flasheos
int contadorFlash = 0;

// Variable booleana que almacena el
// estado de encendido/apagado de todos los leds
bool on_off = false;

void setup() {
  // Establece los pines de cada led como OUTPUT
  for (int i = 0; i < 4; i++) { pinMode(leds[i], OUTPUT); }
  // Estado inicial como APAGADO
  edoLeds = APAGADO;
  // Se establece la velocidad de transferencia del puerto serial
  Serial.begin(BAUD_RATE);
  // Se apagan todos los leds
  apagarLeds();
}

void loop() {
  // Ignora la entrada de comandos si el estado es igual a ERROR
  if (edoLeds != ERROR) {

    // Si hay contenido disponible para la lectura en el puerto serie
    if (Serial.available() > 0) {

      // Obtiene el comando
      String comando = Serial.readStringUntil('\n');
      // Recorta los espacios en blanco de los bordes
      comando.trim();

      // Apaga los leds si el comando es igual a "off"
      if (comando == "off") {
        Serial.println("Comando en ejecución: off");
        // Función de transición
        apagarLeds();
        edoLeds = APAGADO;

      // Si el estado es igual a APAGADO y el comando es igual a "ltr" (left to right),
      // los leds se encienden y apagan de izquierda a derecha
      } else if (comando == "ltr" && edoLeds == APAGADO) {
        Serial.println("Comando en ejecución: ltr");
        edoLeds = LTR;
        // Reinicia el índice del led actual
        contadorLtr = 0;

      // Si el estado es igual a APAGADO y el comando es igual a "rtl" (right to left),
      // los leds se encienden y apagan de derecha a izquierda
      } else if (comando == "rtl" && edoLeds == APAGADO) {
        Serial.println("Comando en ejecución: rtl");
        // Actualiza el estado
        edoLeds = RTL;
        // Reinicia el índice del led actual
        contadorRtl = 3;

      // Si no se ingresó un comando esperado, los leds flashean 10 veces
      } else {
        Serial.print("Comando erróneo: ");
        Serial.println(comando);
        edoLeds = ERROR;
      }
    }
  }
  // Llama a la función de autotransición de cada estado
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
/**
* Apaga todos los leds. Para lograr esto, recorre el arreglo
* que almacena los pines de cada led y apaga cada uno iterativamente.
*/
void apagarLeds() { for (int i = 0; i < 4; i++) { digitalWrite(leds[i], LOW); } }
/**
* Enciende y apaga los leds de izquierda a derecha.
* Cada led dura encendido el tiempo de espera establecido en la
* variable PAUSA_LTR_RTL.
*/
void izquierdaDerecha() {
  // Si ya transcurrió el tiempo de espera
  if (pausaLtrRTL.update()) {

    // El índice corresponde al led inicial (izquierda)
    if (contadorLtr == 0) {
      // Apaga el led del otro extremo (derecha)
      digitalWrite(leds[3], LOW);
      // Enciende el led actual y luego incrementa el índice en 1
      digitalWrite(leds[contadorLtr++], HIGH);

    // El led actual se ubica entre los extremos
    } else if (contadorLtr > 0 && contadorLtr <= 3) {
      // Apaga el led anterior al actual
      digitalWrite(leds[contadorLtr - 1], LOW);
      // Enciende el led actual y luego incrementa el índice en 1
      digitalWrite(leds[contadorLtr++], HIGH);
      // Si el índice es mayor al led del extremo, reinicia el índice
      if (contadorLtr > 3) { contadorLtr = 0; }
    }
  }
}
/**
* Enciende y apaga los leds de derecha a izquierda.
* Cada led dura encendido el tiempo de espera establecido en la
* variable PAUSA_LTR_RTL.
*/
void derechaIzquierda() {
  // Si ya transcurrió el tiempo de espera
  if (pausaLtrRTL.update()) {

    // El índice corresponde al led inicial (derecha)
    if (contadorRtl == 3) {
      // Apaga el led del otro extremo (izquierda)
      digitalWrite(leds[0], LOW);
      // Enciende el led actual y luego decrementa el índice en 1
      digitalWrite(leds[contadorRtl--], HIGH);

    // El led actual se ubica entre los extremos
    } else if (contadorRtl >= 0 && contadorRtl < 3) {
      // Apaga el led anterior al actual
      digitalWrite(leds[contadorRtl + 1], LOW);
      // Enciende el led actual y luego decrementa el índice en 1
      digitalWrite(leds[contadorRtl--], HIGH);
      // Si el índice es mayor al led del extremo, reinicia el índice
      if (contadorRtl < 0) { contadorRtl = 3; }
    }
  }
}
/**
* Hace flashear diez veces todos los leds al mismo tiempo.
* Cada led dura encendido el tiempo de espera establecido en
* la variable PAUSA_ERROR.
*/
void error() {
  // Si ya transcurrió
  if (pausaError.update()) {
    // Si contador es menor a 10 (0 - 9)
    if (contadorFlash < 10) {

      // Si los leds están apagados
      if (!on_off) {
        // Enciende los leds
        for (int i = 0; i < 4; i++) { digitalWrite(leds[i], HIGH); }
        // Incrementa el contador (1 flasheo)
        contadorFlash++;

      // Si están ecendidos, los apaga
      } else { for (int i = 0; i < 4; i++) { digitalWrite(leds[i], LOW); } }

      // Reemplaza la variable booleana por su opuesto
      // (encendido -> apagado o apagado -> encendido)
      on_off = !on_off;

    // Si el contador llegó a 10
    } else {
      // Reinicia el contador de flasheos
      contadorFlash = 0;
      // Leds apagados
      on_off = false;
      // Se apagan los leds y se actualiza el estado del circuito
      apagarLeds();
      edoLeds = APAGADO;
    }
  }
}