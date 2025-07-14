#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include <string>
#include "../assets/Juego.h"

class JuegoGUI {
private:
    // Ventana y renderizado
    sf::RenderWindow window;
    sf::Font font;
    
    // Elementos gráficos del tablero
    std::vector<sf::RectangleShape> casillas;
    std::vector<sf::Text> numerosCasillas;
    std::vector<sf::Text> nombresCasillas;
    
    // Jugadores
    std::vector<sf::CircleShape> fichasJugadores;
    std::vector<sf::Text> nombresJugadores;
    
    // Elementos de UI
    sf::Text tituloJuego;
    sf::Text textoDado;
    sf::Text textoJugadorActual;
    sf::Text textoMensaje;
    sf::RectangleShape botonDado;
    sf::Text textoBotonDado;
    
    // Lógica del juego
    std::unique_ptr<Juego> juego;
    bool juegoIniciado;
    int jugadorSeleccionado;
    
    // Historial de acciones
    std::vector<std::string> historial;
    void agregarAlHistorial(const std::string& accion);
    
    // Constantes
    static const int ANCHO_VENTANA = 1200;
    static const int ALTO_VENTANA = 800;
    static const int TAMANO_CASILLA = 40;
    static const int MARGEN = 50;
    static const int FILAS_TABLERO = 9;
    static const int COLUMNAS_TABLERO = 7;
    
    // Colores
    sf::Color COLOR_FONDO = sf::Color(34, 139, 34); // Verde bosque
    sf::Color COLOR_CASILLA_NORMAL = sf::Color(255, 255, 255); // Blanco
    sf::Color COLOR_CASILLA_OCA = sf::Color(255, 215, 0); // Dorado
    sf::Color COLOR_CASILLA_ESPECIAL = sf::Color(255, 165, 0); // Naranja
    sf::Color COLOR_FICHA = sf::Color(255, 0, 0); // Rojo
    sf::Color COLOR_TEXTO = sf::Color(0, 0, 0); // Negro

public:
    JuegoGUI();
    ~JuegoGUI() = default;
    
    void ejecutar();
    void inicializarJuego(const std::vector<std::string>& nombres);
    void dibujarTablero();
    void dibujarJugadores();
    void dibujarUI();
    void procesarEventos();
    void actualizarJuego();
    void mostrarMensaje(const std::string& mensaje);
    sf::Vector2f obtenerPosicionCasilla(int numeroCasilla);
    void animarMovimiento(int jugadorIndex, int casillaDestino);
}; 