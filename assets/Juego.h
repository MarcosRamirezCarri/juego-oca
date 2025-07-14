#pragma once

#include <vector>
#include <memory>
#include "Jugador.h"
#include "Dado.h"
#include "Tablero.h"
#include <iostream>

class Juego {
private:
    std::vector<Jugador> jugadores;
    std::unique_ptr<Dado> dado;
    std::unique_ptr<Tablero> tablero;
    int cantidadJugadores;
    bool finDelJuego;
    int jugadorActual;
    bool turnoExtra;

public:
    Juego(const std::vector<std::string>& nombresJugadores);
    
    void iniciarJuego();
    void jugarTurno();
    bool verificarGanador() const;
    void mostrarEstadoJuego() const;
    void procesarMovimiento(int jugadorIndex, int nuevaPosicion);
    void liberarJugadoresDelPozo(int jugadorQueCayo);
    bool hayJugadoresEnPozo() const;
    void pasarTurno();
    
    // Métodos para la interfaz gráfica
    const Jugador& obtenerJugador(int index) const;
    int obtenerCantidadJugadores() const;
    int obtenerJugadorActual() const;
    bool estaJugando() const;

    // Nuevo método para la GUI
    std::pair<int, std::string> lanzarDadoYJugarTurno();
}; 