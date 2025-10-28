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
CasillaOca::CasillaOca(int numero, int destino) 
    : Casilla(numero, "Oca"), destinoOca(destino) {
}

void CasillaOca::accionJugador(Jugador& jugador) {
    jugador.moverJugador(destinoOca);
}

string CasillaOca::getDescripcion() const {
    return "Casilla Oca: salta a la siguiente oca y obtienes un turno extra";
}

// Implementación de CasillaPuente
CasillaPuente::CasillaPuente(int numero, int destino_) 
    : Casilla(numero, "Puente"), destino(destino_) {
}

void CasillaPuente::accionJugador(Jugador& jugador) {
    jugador.moverJugador(destino);
}

string CasillaPuente::getDescripcion() const {
    return "Casilla Puente: vas directo a la casilla 12";
}

// Implementación de CasillaPosada
CasillaPosada::CasillaPosada(int numero, int turnos_) 
    : Casilla(numero, "Posada"), turnos(turnos_) {
}

void CasillaPosada::accionJugador(Jugador& jugador) {
    jugador.perderTurnos(turnos);
}

string CasillaPosada::getDescripcion() const {
    return "Casilla Posada: pierdes 1 turno";
}

// Implementación de CasillaPozo
CasillaPozo::CasillaPozo(int numero) 
    : Casilla(numero, "Pozo") {
}

void CasillaPozo::accionJugador(Jugador& jugador) {
    jugador.entrarPozo();
}

string CasillaPozo::getDescripcion() const {
    return "Casilla Pozo: quedas atrapado hasta que otro jugador caiga";
}

// Implementación de CasillaLaberinto
CasillaLaberinto::CasillaLaberinto(int numero, int destino_) 
    : Casilla(numero, "Laberinto"), destino(destino_) {
}

void CasillaLaberinto::accionJugador(Jugador& jugador) {
    jugador.moverJugador(destino);
}

string CasillaLaberinto::getDescripcion() const {
    return "Casilla Laberinto: retrocedes a la casilla 30";
}

// Implementación de CasillaCarcel
CasillaCarcel::CasillaCarcel(int numero, int turnos_) 
    : Casilla(numero, "Cárcel"), turnos(turnos_) {
}

void CasillaCarcel::accionJugador(Jugador& jugador) {
    jugador.perderTurnos(turnos);
}

string CasillaCarcel::getDescripcion() const {
    return "Casilla Carcel: pierdes 2 turnos";
}

// Implementación de CasillaCalavera
CasillaCalavera::CasillaCalavera(int numero, int destino_) 
    : Casilla(numero, "Calavera"), destino(destino_) {
}

void CasillaCalavera::accionJugador(Jugador& jugador) {
    jugador.moverJugador(destino);
}

string CasillaCalavera::getDescripcion() const {
    return "Casilla Calavera: vuelves a la casilla 1";
}

// Implementación de CasillaJardin
CasillaJardin::CasillaJardin(int numero) 
    : Casilla(numero, "Jardín de la Oca") {
}

void CasillaJardin::accionJugador(Jugador& jugador) {
    // Esta casilla se maneja de forma especial en la lógica del juego
    // para verificar si el jugador llegó exactamente
    
}

string CasillaJardin::getDescripcion() const {
    return "Meta final del juego - debes llegar exactamente aquí";
} 