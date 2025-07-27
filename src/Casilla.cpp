#include "Casilla.h"
#include "Jugador.h"
#include <iostream>

using namespace std;
// Implementación de la clase base Casilla
Casilla::Casilla(int numero, const string& nombre) 
    : numeroCasilla(numero), nombre(nombre) {
}

int Casilla::getNumero() const {
    return numeroCasilla;
}

string Casilla::getNombre() const {
    return nombre;
}

// Implementación de CasillaNormal
CasillaNormal::CasillaNormal(int numero) 
    : Casilla(numero, "Casilla Normal") {
}

void CasillaNormal::accionJugador(Jugador& jugador) {
    // No hace nada especial
}

string CasillaNormal::getDescripcion() const {
    return "Casilla normal sin efectos especiales";
}

// Implementación de CasillaOca
CasillaOca::CasillaOca(int numero) 
    : Casilla(numero, "Oca") {
}

void CasillaOca::accionJugador(Jugador& jugador) {
    int posicionActual = jugador.conseguirPosicion();
    int siguienteOca;
    
    // Determinar la siguiente casilla de oca
    if (posicionActual == 54) {
        siguienteOca = 63; // Jardín de la Oca (caso especial)
    } else {
        siguienteOca = posicionActual + 9; // Sumar 9 para ir a la siguiente oca
    }
    
    jugador.moverJugador(siguienteOca);
}

string CasillaOca::getDescripcion() const {
    return "Casilla Oca: salta a la siguiente oca y obtienes un turno extra";
}

// Implementación de CasillaPuente
CasillaPuente::CasillaPuente() 
    : Casilla(6, "Puente") {
}

void CasillaPuente::accionJugador(Jugador& jugador) {
    jugador.moverJugador(12);
}

string CasillaPuente::getDescripcion() const {
    return "Casilla Puente: vas directo a la casilla 12";
}

// Implementación de CasillaPosada
CasillaPosada::CasillaPosada() 
    : Casilla(19, "Posada") {
}

void CasillaPosada::accionJugador(Jugador& jugador) {
    jugador.perderTurnos(1);
}

string CasillaPosada::getDescripcion() const {
    return "Casilla Posada: pierdes 1 turno";
}

// Implementación de CasillaPozo
CasillaPozo::CasillaPozo() 
    : Casilla(31, "Pozo") {
}

void CasillaPozo::accionJugador(Jugador& jugador) {
    jugador.entrarPozo();
}

string CasillaPozo::getDescripcion() const {
    return "Casilla Pozo: quedas atrapado hasta que otro jugador caiga";
}

// Implementación de CasillaLaberinto
CasillaLaberinto::CasillaLaberinto() 
    : Casilla(42, "Laberinto") {
}

void CasillaLaberinto::accionJugador(Jugador& jugador) {
    jugador.moverJugador(30);
}

string CasillaLaberinto::getDescripcion() const {
    return "Casilla Laberinto: retrocedes a la casilla 30";
}

// Implementación de CasillaCarcel
CasillaCarcel::CasillaCarcel() 
    : Casilla(56, "Cárcel") {
}

void CasillaCarcel::accionJugador(Jugador& jugador) {
    jugador.perderTurnos(2);
}

string CasillaCarcel::getDescripcion() const {
    return "Casilla Carcel: pierdes 2 turnos";
}

// Implementación de CasillaCalavera
CasillaCalavera::CasillaCalavera() 
    : Casilla(58, "Calavera") {
}

void CasillaCalavera::accionJugador(Jugador& jugador) {
    jugador.moverJugador(1);
}

string CasillaCalavera::getDescripcion() const {
    return "Casilla Calavera: vuelves a la casilla 1";
}

// Implementación de CasillaJardin
CasillaJardin::CasillaJardin() 
    : Casilla(63, "Jardín de la Oca") {
}

void CasillaJardin::accionJugador(Jugador& jugador) {
    // Esta casilla se maneja de forma especial en la lógica del juego
    // para verificar si el jugador llegó exactamente
    
}

string CasillaJardin::getDescripcion() const {
    return "Meta final del juego - debes llegar exactamente aquí";
} 