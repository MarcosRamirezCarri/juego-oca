#ifndef DADO_H
#define DADO_H

class Dado {
private:
    int carasDelDado;
    int cantidadDados;

public:
    Dado(int caras = 6, int cantidad = 1);
    int lanzar();
    int getCantidadDados() const { return cantidadDados; }
};

#endif // DADO_H

 