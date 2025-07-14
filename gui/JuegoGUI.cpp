#include "JuegoGUI.h"
#include <iostream>
#include <sstream>
#include <algorithm>

JuegoGUI::JuegoGUI() 
    : window(sf::VideoMode(ANCHO_VENTANA, ALTO_VENTANA), "Juego de la Oca - POO"),
      juegoIniciado(false),
      jugadorSeleccionado(0) {
    
    // Configurar ventana
    window.setFramerateLimit(60);
    
    // Cargar fuente - compatible con Windows y Linux
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        if (!font.loadFromFile("/System/Library/Fonts/Arial.ttf")) {
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                if (!font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) {
                    std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
                }
            }
        }
    }
    
    // Inicializar elementos de UI
    tituloJuego.setFont(font);
    tituloJuego.setString("JUEGO DE LA OCA");
    tituloJuego.setCharacterSize(32);
    tituloJuego.setFillColor(sf::Color::White);
    tituloJuego.setPosition(ANCHO_VENTANA / 2 - tituloJuego.getGlobalBounds().width / 2, 10);
    
    textoJugadorActual.setFont(font);
    textoJugadorActual.setCharacterSize(20);
    textoJugadorActual.setFillColor(sf::Color::White);
    textoJugadorActual.setPosition(ANCHO_VENTANA - 300, 50);
    
    textoDado.setFont(font);
    textoDado.setCharacterSize(24);
    textoDado.setFillColor(sf::Color::White);
    textoDado.setPosition(ANCHO_VENTANA - 300, 100);
    
    textoMensaje.setFont(font);
    textoMensaje.setCharacterSize(16);
    textoMensaje.setFillColor(sf::Color::Yellow);
    textoMensaje.setPosition(ANCHO_VENTANA - 300, 150);
    
    // Botón del dado
    botonDado.setSize(sf::Vector2f(120, 50));
    botonDado.setFillColor(sf::Color(70, 130, 180));
    botonDado.setPosition(ANCHO_VENTANA - 300, 200);
    
    textoBotonDado.setFont(font);
    textoBotonDado.setString("LANZAR DADO");
    textoBotonDado.setCharacterSize(16);
    textoBotonDado.setFillColor(sf::Color::White);
    textoBotonDado.setPosition(
        botonDado.getPosition().x + botonDado.getSize().x / 2 - textoBotonDado.getGlobalBounds().width / 2,
        botonDado.getPosition().y + botonDado.getSize().y / 2 - textoBotonDado.getGlobalBounds().height / 2
    );
    
    // Inicializar casillas del tablero
    casillas.resize(64); // 0-63
    numerosCasillas.resize(64);
    nombresCasillas.resize(64);
    
    for (int i = 0; i < 64; i++) {
        casillas[i].setSize(sf::Vector2f(TAMANO_CASILLA, TAMANO_CASILLA));
        casillas[i].setOutlineThickness(2);
        casillas[i].setOutlineColor(sf::Color::Black);
        
        numerosCasillas[i].setFont(font);
        numerosCasillas[i].setString(std::to_string(i));
        numerosCasillas[i].setCharacterSize(10);
        numerosCasillas[i].setFillColor(COLOR_TEXTO);
        
        nombresCasillas[i].setFont(font);
        nombresCasillas[i].setCharacterSize(8);
        nombresCasillas[i].setFillColor(COLOR_TEXTO);
        
        // Posicionar casillas en patrón serpenteante
        sf::Vector2f pos = obtenerPosicionCasilla(i);
        casillas[i].setPosition(pos);
        numerosCasillas[i].setPosition(pos.x + 2, pos.y + 2);
        nombresCasillas[i].setPosition(pos.x + 2, pos.y + TAMANO_CASILLA - 12);
        
        // Colorear casillas especiales
        if (i == 9 || i == 18 || i == 27 || i == 36 || i == 45 || i == 54) {
            casillas[i].setFillColor(COLOR_CASILLA_OCA);
            nombresCasillas[i].setString("OCA");
        } else if (i == 6) {
            casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
            nombresCasillas[i].setString("PUENTE");
        } else if (i == 19) {
            casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
            nombresCasillas[i].setString("POSADA");
        } else if (i == 31) {
            casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
            nombresCasillas[i].setString("POZO");
        } else if (i == 42) {
            casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
            nombresCasillas[i].setString("LABERINTO");
        } else if (i == 56) {
            casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
            nombresCasillas[i].setString("CARCEL");
        } else if (i == 58) {
            casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
            nombresCasillas[i].setString("CALAVERA");
        } else if (i == 63) {
            casillas[i].setFillColor(sf::Color(0, 255, 0)); // Verde para meta
            nombresCasillas[i].setString("META");
        } else {
            casillas[i].setFillColor(COLOR_CASILLA_NORMAL);
        }
    }
    // Inicializar el texto del turno
    textoJugadorActual.setString("Turno: Jugador 1");
}

void JuegoGUI::ejecutar() {
    while (window.isOpen()) {
        procesarEventos();
        actualizarJuego();
        
        window.clear(COLOR_FONDO);
        dibujarTablero();
        dibujarJugadores();
        dibujarUI();
        window.display();
    }
}

void JuegoGUI::inicializarJuego(const std::vector<std::string>& nombres) {
    juego = std::make_unique<Juego>(nombres);
    juegoIniciado = true;
    
    // Inicializar fichas de jugadores
    fichasJugadores.clear();
    nombresJugadores.clear();
    
    std::vector<sf::Color> coloresFichas = {
        sf::Color::Red,
        sf::Color::Blue,
        sf::Color::Green,
        sf::Color::Yellow
    };
    
    for (size_t i = 0; i < nombres.size(); i++) {
        // Ficha del jugador
        sf::CircleShape ficha(15);
        ficha.setFillColor(coloresFichas[i % coloresFichas.size()]);
        ficha.setOutlineThickness(2);
        ficha.setOutlineColor(sf::Color::Black);
        fichasJugadores.push_back(ficha);
        
        // Nombre del jugador
        sf::Text nombre;
        nombre.setFont(font);
        nombre.setString(nombres[i]);
        nombre.setCharacterSize(14);
        nombre.setFillColor(coloresFichas[i % coloresFichas.size()]);
        nombre.setPosition(ANCHO_VENTANA - 300, 300 + i * 30);
        nombresJugadores.push_back(nombre);
    }
    
    mostrarMensaje("¡Juego iniciado! Haz clic en 'LANZAR DADO' para jugar.");
}

void JuegoGUI::dibujarTablero() {
    for (int i = 0; i < 64; i++) {
        window.draw(casillas[i]);
        window.draw(numerosCasillas[i]);
        window.draw(nombresCasillas[i]);
    }
}

void JuegoGUI::dibujarJugadores() {
    if (!juegoIniciado) return;
    
    for (size_t i = 0; i < fichasJugadores.size(); i++) {
        // Posicionar ficha en la casilla del jugador
        int posicion = juego->obtenerJugador(i).conseguirPosicion();
        sf::Vector2f pos = obtenerPosicionCasilla(posicion);
        
        // Ajustar posición para múltiples jugadores en la misma casilla
        pos.x += (i % 2) * 10;
        pos.y += (i / 2) * 10;
        
        fichasJugadores[i].setPosition(pos.x + 5, pos.y + 5);
        window.draw(fichasJugadores[i]);
    }
    
    // Dibujar nombres de jugadores
    for (const auto& nombre : nombresJugadores) {
        window.draw(nombre);
    }
}

void JuegoGUI::dibujarUI() {
    window.draw(tituloJuego);
    window.draw(textoJugadorActual);
    window.draw(textoDado);
    window.draw(textoMensaje);
    window.draw(botonDado);
    window.draw(textoBotonDado);

    // Dibujar historial de acciones
    float yHist = 350;
    sf::Text tHist;
    tHist.setFont(font);
    tHist.setCharacterSize(14);
    tHist.setFillColor(sf::Color::White);
    tHist.setStyle(sf::Text::Regular);
    tHist.setPosition(ANCHO_VENTANA - 300, yHist);
    tHist.setString("Historial:");
    window.draw(tHist);
    yHist += 22;
    for (const auto& linea : historial) {
        tHist.setString(linea);
        tHist.setPosition(ANCHO_VENTANA - 300, yHist);
        window.draw(tHist);
        yHist += 18;
    }
}

void JuegoGUI::procesarEventos() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                // Verificar clic en botón del dado
                if (botonDado.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (juegoIniciado && juego->estaJugando()) {
                        // Usar la lógica real del juego
                        auto [resultado, desc] = juego->lanzarDadoYJugarTurno();
                        textoDado.setString("Dado: " + std::to_string(resultado));
                        mostrarMensaje(desc);
                        agregarAlHistorial(desc);
                        // Actualizar el texto del turno
                        int actual = juego->obtenerJugadorActual();
                        textoJugadorActual.setString("Turno: Jugador " + std::to_string(actual + 1));
                    }
                }
            }
        }
    }
}

void JuegoGUI::actualizarJuego() {
    // Aquí se actualizaría la lógica del juego
}

void JuegoGUI::mostrarMensaje(const std::string& mensaje) {
    textoMensaje.setString(mensaje);
}

sf::Vector2f JuegoGUI::obtenerPosicionCasilla(int numeroCasilla) {
    // Patrón serpenteante: de abajo hacia arriba, alternando direcciones
    int fila = numeroCasilla / COLUMNAS_TABLERO;
    int columna = numeroCasilla % COLUMNAS_TABLERO;
    
    // Invertir dirección en filas impares
    if (fila % 2 == 1) {
        columna = COLUMNAS_TABLERO - 1 - columna;
    }
    
    float x = MARGEN + columna * TAMANO_CASILLA;
    float y = ALTO_VENTANA - MARGEN - (fila + 1) * TAMANO_CASILLA;
    
    return sf::Vector2f(x, y);
}

void JuegoGUI::animarMovimiento(int jugadorIndex, int casillaDestino) {
    // Aquí se implementaría la animación del movimiento
    // Por simplicidad, por ahora solo actualizamos la posición
    sf::Vector2f pos = obtenerPosicionCasilla(casillaDestino);
    fichasJugadores[jugadorIndex].setPosition(pos.x + 5, pos.y + 5);
} 

void JuegoGUI::agregarAlHistorial(const std::string& accion) {
    historial.push_back(accion);
    if (historial.size() > 10) {
        historial.erase(historial.begin());
    }
} 