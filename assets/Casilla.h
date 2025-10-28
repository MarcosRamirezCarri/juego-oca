#ifndef CASILLA_H
#define CASILLA_H

#include <string>

using namespace std;

class Jugador;

class Casilla {
protected:
    int numeroCasilla;
    string nombre;
    // Nota: Las casillas especiales pueden necesitar parámetros adicionales

public:
    Casilla(int numero, const string& nombre);
    virtual ~Casilla() = default;
    
    int getNumero() const;
    string getNombre() const;
    virtual void accionJugador(Jugador& jugador) = 0;
    virtual string getDescripcion() const = 0;
};

// Casilla normal (sin efectos especiales)
class CasillaNormal : public Casilla {
public:
    CasillaNormal(int numero);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla de Oca
class CasillaOca : public Casilla {
private:
    int destinoOca; // siguiente casilla de oca (o meta)
public:
    CasillaOca(int numero, int destino);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Puente
class CasillaPuente : public Casilla {
private:
    int destino;
public:
    CasillaPuente(int numero, int destino);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Posada
class CasillaPosada : public Casilla {
private:
    int turnos;
public:
    CasillaPosada(int numero, int turnos = 1);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Pozo
class CasillaPozo : public Casilla {
public:
    CasillaPozo(int numero);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Laberinto
class CasillaLaberinto : public Casilla {
private:
    int destino;
public:
    CasillaLaberinto(int numero, int destino);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Cárcel
class CasillaCarcel : public Casilla {
private:
    int turnos;
public:
    CasillaCarcel(int numero, int turnos = 2);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Calavera
class CasillaCalavera : public Casilla {
private:
    int destino;
public:
    CasillaCalavera(int numero, int destino = 1);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

// Casilla Jardín de la Oca
class CasillaJardin : public Casilla {
public:
    CasillaJardin(int numero);
    void accionJugador(Jugador& jugador) override;
    string getDescripcion() const override;
};

#endif