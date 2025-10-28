#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include <string>
#include <memory>
#include "Jugador.h"
#include "Casilla.h"
#include "Dado.h"


struct ResultadoTurno {
    int resultadoDado;
    string descripcion;
    
    ResultadoTurno(int dado, const string& desc) 
        : resultadoDado(dado), descripcion(desc) {}
};

class JuegoGUI; // Forward declaration

class Juego {
private:
    vector<Jugador> jugadores;
    vector<unique_ptr<Casilla>> casillas;  // Casillas directamente en Juego
    unique_ptr<Dado> dado;
    int cantidadJugadores;
    int jugadorActual;
    bool finDelJuego;
    bool turnoExtra;
    JuegoGUI* gui;
    int meta; // última casilla (meta)
    bool especialesAleatorios;
    int posicionPozo;
    
    // Métodos privados para manejar el tablero
    void inicializarCasillas();
    Casilla* obtenerCasilla(int numero) const;

public:
    Juego(const vector<string>& nombresJugadores, int meta = 63, bool especialesAleatorios = false);
    ~Juego();
    
    void iniciarJuego();
    void jugarTurno();
    bool verificarGanador() const;
    void procesarMovimiento(int jugadorIndex, int nuevaPosicion);
    void liberarJugadoresDelPozo(int jugadorQueCayo);
    bool hayJugadoresEnPozo() const;
    void pasarTurno();
    
    // Métodos para la interfaz gráfica
    const Jugador& obtenerJugador(int index) const;
    int obtenerCantidadJugadores() const;
    int obtenerJugadorActual() const;
    bool estaJugando() const;
    int obtenerMeta() const { return meta; }
    string obtenerNombreCasilla(int numero) const;
    
    //Metodo para reiniciar el juego
    void reiniciarJuego();

    // Metodo para lanzar el dado, jugar el turno y devolver el resultado (informa a la interfaz gráfica)
    ResultadoTurno lanzarDadoYJugarTurno();
    
    // Método para conectar con la interfaz gráfica
    void setGUI(JuegoGUI* interfaz);

};

#endif // JUEGO_H 