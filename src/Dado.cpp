#include "Dado.h"
#include <cstdlib>

Dado::Dado(int caras, int cantidad) : carasDelDado(caras), cantidadDados(cantidad) {
    // El constructor ya está implementado con la lista de inicialización
}

int Dado::lanzar() {
    int total = 0;
    for (int i = 0; i < cantidadDados; ++i) {
        total += (std::rand() % carasDelDado) + 1;
    }
    return total;
} 