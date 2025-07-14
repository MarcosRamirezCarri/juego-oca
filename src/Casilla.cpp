#include "Casilla.h"
#include "Jugador.h"
#include <iostream>

// Implementación de la clase base Casilla
Casilla::Casilla(int numero, const std::string& nombre) 
    : numeroCasilla(numero), nombre(nombre) {
}

int Casilla::getNumero() const {
    return numeroCasilla;
}

std::string Casilla::getNombre() const {
    return nombre;
}

// Implementación de CasillaNormal
CasillaNormal::CasillaNormal(int numero) 
    : Casilla(numero, "Casilla Normal") {
}

void CasillaNormal::accionJugador(Jugador& jugador) {
    // No hace nada especial
}

std::string CasillaNormal::getDescripcion() const {
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
    if (posicionActual == 9) siguienteOca = 18;
    else if (posicionActual == 18) siguienteOca = 27;
    else if (posicionActual == 27) siguienteOca = 36;
    else if (posicionActual == 36) siguienteOca = 45;
    else if (posicionActual == 45) siguienteOca = 54;
    else if (posicionActual == 54) siguienteOca = 63; // Jardín de la Oca
    else siguienteOca = 63; // Por si acaso
    
    jugador.moverJugador(siguienteOca);
    std::cout << "¡De oca a oca y tiro porque me toca! Avanzas a la casilla " << siguienteOca << std::endl;
}

std::string CasillaOca::getDescripcion() const {
    return "Salta a la siguiente oca y obtienes un turno extra";
}

// Implementación de CasillaPuente
CasillaPuente::CasillaPuente() 
    : Casilla(6, "Puente") {
}

void CasillaPuente::accionJugador(Jugador& jugador) {
    jugador.moverJugador(12);
    std::cout << "¡Has caído en el puente! Vas directamente a la casilla 12 (Posada)" << std::endl;
}

std::string CasillaPuente::getDescripcion() const {
    return "Te lleva directamente a la casilla 12";
}

// Implementación de CasillaPosada
CasillaPosada::CasillaPosada() 
    : Casilla(19, "Posada") {
}

void CasillaPosada::accionJugador(Jugador& jugador) {
    jugador.perderTurnos(1);
    std::cout << "¡Has caído en la posada! Pierdes 1 turno" << std::endl;
}

std::string CasillaPosada::getDescripcion() const {
    return "Pierdes 1 turno";
}

// Implementación de CasillaPozo
CasillaPozo::CasillaPozo() 
    : Casilla(31, "Pozo") {
}

void CasillaPozo::accionJugador(Jugador& jugador) {
    jugador.entrarPozo();
    std::cout << "¡Has caído en el pozo! Quedas atrapado hasta que otro jugador caiga aquí" << std::endl;
}

std::string CasillaPozo::getDescripcion() const {
    return "Quedas atrapado hasta que otro jugador caiga aquí";
}

// Implementación de CasillaLaberinto
CasillaLaberinto::CasillaLaberinto() 
    : Casilla(42, "Laberinto") {
}

void CasillaLaberinto::accionJugador(Jugador& jugador) {
    jugador.moverJugador(30);
    std::cout << "¡Has entrado en el laberinto! Retrocedes a la casilla 30" << std::endl;
}

std::string CasillaLaberinto::getDescripcion() const {
    return "Retrocedes a la casilla 30";
}

// Implementación de CasillaCarcel
CasillaCarcel::CasillaCarcel() 
    : Casilla(56, "Cárcel") {
}

void CasillaCarcel::accionJugador(Jugador& jugador) {
    jugador.perderTurnos(2);
    std::cout << "¡Has caído en la cárcel! Pierdes 2 turnos" << std::endl;
}

std::string CasillaCarcel::getDescripcion() const {
    return "Pierdes 2 turnos";
}

// Implementación de CasillaCalavera
CasillaCalavera::CasillaCalavera() 
    : Casilla(58, "Calavera") {
}

void CasillaCalavera::accionJugador(Jugador& jugador) {
    jugador.moverJugador(1);
    std::cout << "¡Has caído en la calavera! Vuelves a la casilla 1" << std::endl;
}

std::string CasillaCalavera::getDescripcion() const {
    return "Vuelves a la casilla 1";
}

// Implementación de CasillaJardin
CasillaJardin::CasillaJardin() 
    : Casilla(63, "Jardín de la Oca") {
}

void CasillaJardin::accionJugador(Jugador& jugador) {
    // Esta casilla se maneja de forma especial en la lógica del juego
    // para verificar si el jugador llegó exactamente
    std::cout << "¡Has llegado al Jardín de la Oca!" << std::endl;
}

std::string CasillaJardin::getDescripcion() const {
    return "Meta final del juego - debes llegar exactamente aquí";
} 