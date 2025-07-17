#pragma once

#include <vector>
#include <memory>
#include "Jugador.h"
#include "Dado.h"
#include "Tablero.h"
#include <iostream>

using namespace std;

// Forward declaration para evitar dependencias circulares
class JuegoGUI;

class Juego {
private:
    vector<Jugador> jugadores;
    unique_ptr<Dado> dado;
    unique_ptr<Tablero> tablero;
    int cantidadJugadores;
    bool finDelJuego;
    int jugadorActual;
    bool turnoExtra;
    
    // Referencia a la interfaz gráfica (opcional)
    JuegoGUI* gui;

public:
    Juego(const vector<string>& nombresJugadores);
    ~Juego();
    
    // Métodos principales del juego
    void iniciarJuego();
    void jugarTurno();
    bool verificarGanador() const;
    void mostrarEstadoJuego() const;
    void procesarMovimiento(int jugadorIndex, int nuevaPosicion);
    void liberarJugadoresDelPozo(int jugadorQueCayo);
    bool hayJugadoresEnPozo() const;
    void pasarTurno();
    
    // Métodos de acceso
    const Jugador& obtenerJugador(int index) const;
    int obtenerCantidadJugadores() const;
    int obtenerJugadorActual() const;
    bool estaJugando() const;
    pair<int, string> lanzarDadoYJugarTurno();
    
    // Métodos para conectar con la interfaz gráfica
    void setGUI(JuegoGUI* interfaz);
    void notificarCambioTurno();
    void notificarMovimiento(int jugadorIndex, int posicionAnterior, int nuevaPosicion);
    void notificarGanador(const string& nombreGanador);
}; 