#include "JuegoGUI.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    
    try {
        // Crear la interfaz gráfica del juego
        JuegoGUI gui;
        
        // Ejecutar el juego gráfico (la GUI se encargará de pedir la configuración)
        gui.ejecutar();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
} 