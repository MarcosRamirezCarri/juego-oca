#include "JuegoGUI.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Inicializar semilla para números aleatorios
    srand(time(0));
    
    try {
        // Crear la interfaz gráfica
        JuegoGUI gui;
        
        // Configurar jugadores (por ahora hardcodeados, se puede hacer interactivo)
        std::vector<std::string> nombresJugadores = {"Ana", "Carlos", "María"};
        
        // Inicializar el juego
        gui.inicializarJuego(nombresJugadores);
        
        // Ejecutar el juego
        gui.ejecutar();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 