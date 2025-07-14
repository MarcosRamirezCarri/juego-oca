#ifndef CASILLA_H
#define CASILLA_H

#include <string>

class Jugador;

class Casilla {
protected:
    int numeroCasilla;
    std::string nombre;

public:
    Casilla(int numero, const std::string& nombre);
    virtual ~Casilla() = default;
    
    int getNumero() const;
    std::string getNombre() const;
    virtual void accionJugador(Jugador& jugador) = 0;
    virtual std::string getDescripcion() const = 0;
};

// Casilla normal (sin efectos especiales)
class CasillaNormal : public Casilla {
public:
    CasillaNormal(int numero);
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla de Oca
class CasillaOca : public Casilla {
public:
    CasillaOca(int numero);
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Puente
class CasillaPuente : public Casilla {
public:
    CasillaPuente();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Posada
class CasillaPosada : public Casilla {
public:
    CasillaPosada();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Pozo
class CasillaPozo : public Casilla {
public:
    CasillaPozo();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Laberinto
class CasillaLaberinto : public Casilla {
public:
    CasillaLaberinto();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Cárcel
class CasillaCarcel : public Casilla {
public:
    CasillaCarcel();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Calavera
class CasillaCalavera : public Casilla {
public:
    CasillaCalavera();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

// Casilla Jardín de la Oca
class CasillaJardin : public Casilla {
public:
    CasillaJardin();
    void accionJugador(Jugador& jugador) override;
    std::string getDescripcion() const override;
};

#endif