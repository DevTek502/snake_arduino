/************************
** @Autor: Edwin Lopez **
*************************/ 

// Librerias Utilizadas
#include "LedControl.h"


// pines  max7219
const int DIN = 12;
const int CLK = 11;
const int CS = 10;

LedControl lc = LedControl(DIN, CLK, CS, 1);

// variables pantalla
const int anchoPantalla = 8;
const int altoPantalla = 8;

// variables snake
int puntuacion = 0, snakeTamano = 1;

int snakeX, snakeY, comidaX, comidaY;

int anteriorX, anteriorY, temporalX, temporalY;

int colaX[100], colaY[100];

char direccion;

bool juegoTerminado = false;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// SETUP
void setup() {
    configurarMatriz();
    configurarControl();
    configurarPosicionInicial();
    configurarPosicionComida();
}

// configuracion inicial de la matriz
void configurarMatriz() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);
}

// configuracion inicial de los controles
void configurarControl() {
    attachInterrupt(digitalPinToInterrupt(21), pulsacionArriba, HIGH);
    attachInterrupt(digitalPinToInterrupt(20), pulsacionAbajo, HIGH);
    attachInterrupt(digitalPinToInterrupt(18), pulsacionIzquieda, HIGH);
    attachInterrupt(digitalPinToInterrupt(19), pulsacionDerecha, HIGH);
}

// interrupciones para el control (Joystick)
void pulsacionArriba() {
    if (direccion != 'd') direccion = 'u';
}

void pulsacionAbajo() {
    if (direccion != 'u') direccion = 'd';
}

void pulsacionIzquieda() {
    if (direccion != 'r') direccion = 'l';
}

void pulsacionDerecha() {
    if (direccion != 'l') direccion = 'r';
}

// configuracion inicial para el snake
void configurarPosicionInicial() {
    snakeX = 4;
    snakeY = 4;
    encenderLed(snakeX, snakeY);
}

// configuracion para la comida
void configurarPosicionComida() {
    comidaX = rand() % anchoPantalla;
    comidaY = rand() % altoPantalla;
    encenderLed(comidaX, comidaY);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// LOOP
void loop() {
    if (juegoTerminado) {
        animacionFinJuego();
    } else {
        iniciarJuego();
    }
}

// metodo encargado de ejecutar el juego
void iniciarJuego() {
    verificarFinJuego();
    cambiarPosicion();
    verificarBordes();
    verificarChoqueComida();
    manejadorCola();
    dibujarSnake();
    delay(150);
}

// metodo encargado de verificar el fin del juego
void verificarFinJuego() {
    for (int i = 1; i < snakeTamano; i++) {
        if (colaX[i] == snakeX && colaY[i] == snakeY) {
            juegoTerminado = true;
        }
    }
}

// metodo encargado de verificar los bordes del juego
void verificarBordes() {
    if (snakeX >= anchoPantalla) {
        snakeX = 0;
    } else if (snakeX < 0) {
        snakeX = anchoPantalla - 1;
    }
    if (snakeY >= altoPantalla) {
        snakeY = 0;
    } else if (snakeY < 0) {
        snakeY = altoPantalla - 1;
    }
}

// metodo encargado de manejar la cola del snake
void manejadorCola() {
    anteriorX = colaX[0];
    anteriorY = colaY[0];
    colaX[0] = snakeX;
    colaY[0] = snakeY;

    for (int i = 1; i < snakeTamano; i++) {
        temporalX = colaX[i];
        temporalY = colaY[i];
        colaX[i] = anteriorX;
        colaY[i] = anteriorY;
        anteriorX = temporalX;
        anteriorY = temporalY;
    }
}

// metodo encargado de verficicar coliciones con la comida
void verificarChoqueComida() {
    if (snakeX == comidaX && snakeY == comidaY) {
        puntuacion++;
        snakeTamano++;
        configurarPosicionComida();
    }
}

// metodo encargado de cambia la posicion
void cambiarPosicion() {
    switch (direccion) {
        case 'l':
            snakeX--;
            break;
        case 'r':
            snakeX++;
            break;
        case 'u':
            snakeY--;
            break;
        case 'd':
            snakeY++;
            break;
    }
}

// metodo encargado de mostrar la animacion de fin del juego
void animacionFinJuego() {
    for (int i = 0; i < altoPantalla; i++) {
        for (int j = 0; j < anchoPantalla; j++) {
            encenderLed(j, i);
            delay(50);
        }
    }
    reiniciarVariables();
}

// metodo encargado de reiniciar variables del juego
void reiniciarVariables() {
    lc.clearDisplay(0);
    configurarPosicionInicial();
    configurarPosicionComida();
    direccion = ' ';
    juegoTerminado = false;
    puntuacion = 0;
    snakeTamano = 1;
}

// metodo encargado de encender un led
void encenderLed(int fila, int columna) { lc.setLed(0, fila, columna, true); }

// metodo encargado de apagar un led
void apagarLed(int fila, int columna) { lc.setLed(0, fila, columna, false); }

// metodo encargado de dibujar el snake
void dibujarSnake() {
    apagarLed(anteriorX, anteriorY);
    encenderLed(snakeX, snakeY);
    encenderLed(comidaX, comidaY);
}
