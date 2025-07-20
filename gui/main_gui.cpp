#include "JuegoGUI.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    // Inicializar semilla para números aleatorios
    srand(time(0));
    
    try {
        // Crear la interfaz gráfica del juego
        JuegoGUI gui;
        
        // Configurar jugadores (por ahora hardcodeados)
        vector<string> nombresJugadores = {"Marcos", "Valen", "Lucas", "Bauti"};
        
        // Inicializar el juego con la interfaz gráfica
        gui.inicializarJuego(nombresJugadores);
        
        // Ejecutar el juego gráfico
        gui.ejecutar();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
} 