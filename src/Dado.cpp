#include "Dado.h"
#include <cstdlib>
#include <ctime>

Dado::Dado(int caras) : carasDelDado(caras) {
    // El constructor ya está implementado con la lista de inicialización
}

int Dado::lanzar() {
    return (rand() % carasDelDado) + 1;
} 