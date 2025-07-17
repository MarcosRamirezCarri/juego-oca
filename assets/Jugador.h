#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

using namespace std;

class Jugador {
private:
    string nombre;
    int posicion;
    int turnosPerdidos;
    bool enPozo;

public:
    Jugador(const string& nombre);
    
    // Métodos para obtener información
    string conseguirNombre() const;
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

 