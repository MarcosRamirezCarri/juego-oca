#include "Tablero.h"
#include <iostream>

Tablero::Tablero() {
    inicializarCasillas();
}

void Tablero::inicializarCasillas() {
    casillas.resize(NUMERO_CASILLAS + 1); // +1 porque las casillas van de 1 a 63
    
    // Inicializar todas las casillas como normales
    for (int i = 1; i <= NUMERO_CASILLAS; i++) {
        casillas[i] = std::make_unique<CasillaNormal>(i);
    }
    
    // Configurar casillas especiales
    casillas[6] = std::make_unique<CasillaPuente>();
    casillas[9] = std::make_unique<CasillaOca>(9);
    casillas[12] = std::make_unique<CasillaPosada>(); // Posada en casilla 12
    casillas[18] = std::make_unique<CasillaOca>(18);
    casillas[19] = std::make_unique<CasillaPosada>(); // Posada en casilla 19
    casillas[27] = std::make_unique<CasillaOca>(27);
    casillas[30] = std::make_unique<CasillaNormal>(30); // Casilla normal (destino del laberinto)
    casillas[31] = std::make_unique<CasillaPozo>();
    casillas[36] = std::make_unique<CasillaOca>(36);
    casillas[42] = std::make_unique<CasillaLaberinto>();
    casillas[45] = std::make_unique<CasillaOca>(45);
    casillas[54] = std::make_unique<CasillaOca>(54);
    casillas[56] = std::make_unique<CasillaCarcel>();
    casillas[58] = std::make_unique<CasillaCalavera>();
    casillas[63] = std::make_unique<CasillaJardin>();
    
    std::cout << "Tablero inicializado con " << NUMERO_CASILLAS << " casillas" << std::endl;
}

Casilla* Tablero::obtenerCasilla(int numero) const {
    if (numero >= 1 && numero <= NUMERO_CASILLAS) {
        return casillas[numero].get();
    }
    return nullptr;
}

int Tablero::getNumeroCasillas() const {
    return NUMERO_CASILLAS;
}