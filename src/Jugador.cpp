#include "Jugador.h"

Jugador::Jugador(const std::string& nombre) 
    : nombre(nombre), posicion(0), turnosPerdidos(0), enPozo(false) {
}

std::string Jugador::conseguirNombre() const {
    return nombre;
}

int Jugador::conseguirPosicion() const {
    return posicion;
}

int Jugador::getTurnosPerdidos() const {
    return turnosPerdidos;
}

bool Jugador::estaEnPozo() const {
    return enPozo;
}

void Jugador::moverJugador(int nuevaPosicion) {
    posicion = nuevaPosicion;
}

void Jugador::perderTurnos(int turnos) {
    turnosPerdidos += turnos;
}

void Jugador::entrarPozo() {
    enPozo = true;
}

void Jugador::salirPozo() {
    enPozo = false;
}

void Jugador::reducirTurnosPerdidos() {
    // Error típico: no verificar si hay turnos perdidos
    turnosPerdidos--;
}

bool Jugador::puedeJugar() const {
    return turnosPerdidos == 0 && !enPozo;
} 