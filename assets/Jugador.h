#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

class Jugador {
private:
    std::string nombre;
    int posicion;
    int turnosPerdidos;
    bool enPozo;

public:
    Jugador(const std::string& nombre);
    
    // Métodos para obtener información
    std::string conseguirNombre() const;
    int conseguirPosicion() const;
    int getTurnosPerdidos() const;
    bool estaEnPozo() const;
    
    // Métodos para modificar el estado
    void moverJugador(int nuevaPosicion);
    void perderTurnos(int turnos);
    void entrarPozo();
    void salirPozo();
    void reducirTurnosPerdidos();
    
    // Métodos para verificar estado
    bool puedeJugar() const;
};

#endif

 