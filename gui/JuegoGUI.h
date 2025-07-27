#ifndef JUEGO_GUI_H
#define JUEGO_GUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include <string>
#include "../assets/Juego.h"

using namespace std;

class JuegoGUI {
private:
    // Ventana y renderizado
    sf::RenderWindow window;
    sf::Font font;
    
    // Elementos gráficos del tablero
    vector<sf::RectangleShape> casillas;
    vector<sf::Text> numerosCasillas;
    vector<sf::Text> nombresCasillas;
    
    // Jugadores
    vector<sf::CircleShape> fichasJugadores;
    vector<sf::Text> nombresJugadores;
    
    // Elementos de UI
    sf::Text tituloJuego;
    sf::Text textoDado;
    sf::Text textoJugadorActual;
    sf::Text textoMensaje;
    sf::RectangleShape botonDado;
    sf::Text textoBotonDado;
    sf::RectangleShape botonReiniciarJuego;
    sf::Text textoBotonReiniciar;
    sf::Text textoAutores;
    
    // Elementos de configuración inicial
    sf::Text textoConfiguracion;
    sf::Text textoCantidadJugadores;
    sf::RectangleShape botonMenosJugadores;
    sf::RectangleShape botonMasJugadores;
    sf::Text textoMenosJugadores;
    sf::Text textoMasJugadores;
    sf::RectangleShape botonIniciarJuego;
    sf::Text textoIniciarJuego;
    vector<sf::Text> camposNombres;
    vector<sf::RectangleShape> rectangulosNombres;
    int cantidadJugadoresSeleccionada;
    bool enConfiguracion;
    
    // Lógica del juego - ahora es una composición, no herencia
    unique_ptr<Juego> juego;
    bool juegoIniciado;
    int jugadorSeleccionado;
    
    // Historial de acciones
    vector<string> historial;
    void agregarAlHistorial(const string& accion);
    
    // Función auxiliar para configurar elementos de texto
    void configurarTexto(sf::Text& texto, const string& contenido, int tamano, const sf::Color& color, float x, float y);
    
    // Constantes
    static const int ANCHO_VENTANA = 1500;
    static const int ALTO_VENTANA = 900;
    static const int TAMANO_CASILLA = 78;
    static const int MARGEN = 50;
    static const int ESPACIADO_CASILLAS = 6; 
    static const int FILAS_TABLERO = 10;
    static const int COLUMNAS_TABLERO = 9;
    
    // Colores
    sf::Color COLOR_FONDO = sf::Color(34, 139, 34); // Verde bosque
    sf::Color COLOR_CASILLA_NORMAL = sf::Color(255, 255, 255); // Blanco
    sf::Color COLOR_CASILLA_OCA = sf::Color(255, 215, 0); // Dorado
    sf::Color COLOR_CASILLA_ESPECIAL = sf::Color(255, 120, 0); // Naranja
    sf::Color COLOR_CASILLA_CALAVERA = sf::Color(153, 122, 111); // Gris
    sf::Color COLOR_CASILLA_LABERINTO = sf::Color(69, 128, 252); // Azul
    sf::Color COLOR_CASILLA_POZO = sf::Color(255, 38, 38); // Rojo
    sf::Color COLOR_CASILLA_CARCEL = sf::Color(69, 69, 69); // Gris oscuro
    sf::Color COLOR_CASILLA_META = sf::Color(49, 255, 0); // Verde
    sf::Color COLOR_TEXTO = sf::Color(0, 0, 0); // Negro
    sf::Color COLOR_BOTON_REINICIAR = sf::Color(204, 122, 250); // Magenta

public:
    JuegoGUI();
    ~JuegoGUI() = default;
    
    // Métodos principales
    void inicializarConfiguracion();
    void ejecutar();
    void inicializarJuego(const vector<string>& nombres);
    void reiniciarJuego();
    
    // Métodos de configuración
    void procesarEventosConfiguracion();
    void cambiarCantidadJugadores(int cambio);
    void iniciarJuegoDesdeConfiguracion();
    void actualizarCamposNombres();
    
    // Métodos de renderizado
    void dibujarTablero();
    void dibujarConfiguracion();
    void dibujarJugadores();
    void dibujarUI();
    
    // Métodos de manejo de eventos
    void procesarEventos();
    void actualizarJuego();
    void mostrarMensaje(const string& mensaje);
    sf::Vector2f obtenerPosicionCasilla(int numeroCasilla);
    

};

#endif // JUEGO_GUI_H 