#pragma once

#include <vector>
#include <memory>
#include "Casilla.h"

using namespace std;

class Tablero {
private:
    vector<unique_ptr<Casilla>> casillas;
    static const int NUMERO_CASILLAS = 63;

public:
    Tablero();
    
    Casilla* obtenerCasilla(int numero) const;
    void inicializarCasillas();
    int getNumeroCasillas() const;
}; 