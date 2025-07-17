#include "Jugador.h"

using namespace std;

Jugador::Jugador(const string& nombre)
    : nombre(nombre), posicion(0), turnosPerdidos(0), enPozo(false) {
}

string Jugador::conseguirNombre() const {
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
    turnosPerdidos--;
}

bool Jugador::puedeJugar() const {
    return turnosPerdidos == 0 && !enPozo;
} 