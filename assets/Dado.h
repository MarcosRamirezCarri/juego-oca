#ifndef DADO_H
#define DADO_H

class Dado {
private:
    int carasDelDado;

public:
    Dado(int caras = 6);
    int lanzar();
};

#endif // DADO_H

 