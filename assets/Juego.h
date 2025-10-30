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
    int cantidadDados;
    uint32_t semillaAleatoria; // para reproducir el tablero aleatorio
    
    // Métodos privados para manejar el tablero
    void inicializarCasillas();
    Casilla* obtenerCasilla(int numero) const;

public:
    Juego(const vector<string>& nombresJugadores, int meta = 63, bool especialesAleatorios = false, int cantidadDados = 1);
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
    int obtenerCantidadDados() const { return cantidadDados; }
    bool obtenerEspecialesAleatorios() const { return especialesAleatorios; }
    uint32_t obtenerSemillaAleatoria() const { return semillaAleatoria; }
    string obtenerNombreCasilla(int numero) const;
    
    //Metodo para reiniciar el juego
    void reiniciarJuego();

    // Metodo para lanzar el dado, jugar el turno y devolver el resultado (informa a la interfaz gráfica)
    ResultadoTurno lanzarDadoYJugarTurno();
    
    // Método para conectar con la interfaz gráfica
    void setGUI(JuegoGUI* interfaz);

    // Persistencia binaria de la partida (incluye historial)
    bool guardarPartida(const std::string& rutaArchivo, const std::vector<std::string>& historial) const;
    static std::unique_ptr<Juego> cargarPartida(const std::string& rutaArchivo, std::vector<std::string>& historialOut);

};

#endif // JUEGO_H 