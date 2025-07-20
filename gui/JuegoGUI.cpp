#include "JuegoGUI.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

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
                    cerr << "Error: No se pudo cargar la fuente" << endl;
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
    textoJugadorActual.setPosition(ANCHO_VENTANA - 500, 50);
    
    textoDado.setFont(font);
    textoDado.setCharacterSize(24);
    textoDado.setFillColor(sf::Color::White);
    textoDado.setPosition(ANCHO_VENTANA - 500, 100);
    
    textoMensaje.setFont(font);
    textoMensaje.setCharacterSize(16);
    textoMensaje.setFillColor(sf::Color::Yellow);
    textoMensaje.setPosition(ANCHO_VENTANA - 500, 150);
    
    // Botón del dado
    botonDado.setSize(sf::Vector2f(120, 50));
    botonDado.setFillColor(sf::Color(70, 130, 180));
    botonDado.setPosition(ANCHO_VENTANA - 900, 100);
    
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
        numerosCasillas[i].setString(to_string(i));
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
    textoJugadorActual.setString("Turno: Esperando inicio del juego");
}

void JuegoGUI::ejecutar() {
    while (window.isOpen()) {
        procesarEventos();

        
        window.clear(COLOR_FONDO);
        dibujarTablero();
        dibujarJugadores();
        dibujarUI();
        window.display();
    }
}

void JuegoGUI::inicializarJuego(const vector<string>& nombres) {
    juego = make_unique<Juego>(nombres);
    juego->setGUI(this);  // Conectar la GUI con el juego
    juegoIniciado = true;
    
    // Inicializar fichas de jugadores
    fichasJugadores.clear();
    nombresJugadores.clear();
    inicialesJugadores.clear();
    
    vector<sf::Color> coloresFichas = {
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
        
        // Inicial del jugador en la ficha
        sf::Text inicial;
        inicial.setFont(font);
        inicial.setString(string(1, nombres[i][0])); // Primera letra del nombre
        inicial.setCharacterSize(12);
        inicial.setFillColor(sf::Color::Black);
        inicial.setStyle(sf::Text::Bold);
        // La posición se ajustará en dibujarJugadores()
        inicialesJugadores.push_back(inicial);
        
        // Nombre del jugador
        sf::Text nombre ;
        nombre.setFont(font);
        nombre.setString(nombres[i]);
        nombre.setCharacterSize(20);
        nombre.setFillColor(coloresFichas[i % coloresFichas.size()]);
        nombre.setPosition(ANCHO_VENTANA - 500, 250 + i * 30);
        nombresJugadores.push_back(nombre);
    }
    
    // Actualizar el texto del turno inicial
    int actual = juego->obtenerJugadorActual();
    string turnoText = "Turno: " + juego->obtenerJugador(actual).conseguirNombre();
    textoJugadorActual.setString(turnoText);
    
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
        
        // Posicionar y dibujar la inicial centrada en la ficha
        sf::FloatRect fichaBounds = fichasJugadores[i].getGlobalBounds();
        sf::FloatRect textoBounds = inicialesJugadores[i].getGlobalBounds();
        
        float x = fichaBounds.left + (fichaBounds.width - textoBounds.width) / 2;
        float y = fichaBounds.top + (fichaBounds.height - textoBounds.height) / 2;
        
        inicialesJugadores[i].setPosition(x, y);
        window.draw(inicialesJugadores[i]);
    }
    
    // Dibujar nombres de jugadores con sus posiciones actuales
    for (size_t i = 0; i < nombresJugadores.size(); i++) {
        // Actualizar el texto con la posición actual
        int posicion = juego->obtenerJugador(i).conseguirPosicion();
        string nombreCompleto = nombresJugadores[i].getString().toAnsiString();
        
        // Extraer solo el nombre (sin la posición anterior)
        size_t posParentesis = nombreCompleto.find(" (Casilla");
        if (posParentesis != string::npos) {
            nombreCompleto = nombreCompleto.substr(0, posParentesis);
        }
        
        // Agregar la posición actual
        nombreCompleto += " (Casilla " + to_string(posicion) + ")";
        
        // Agregar información de estado si es necesario
        if (juego->obtenerJugador(i).estaEnPozo()) {
            nombreCompleto += " - EN POZO";
        }
        if (juego->obtenerJugador(i).getTurnosPerdidos() > 0) {
            nombreCompleto += " - Pierde " + to_string(juego->obtenerJugador(i).getTurnosPerdidos()) + " turnos";
        }
        
        nombresJugadores[i].setString(nombreCompleto);
        window.draw(nombresJugadores[i]);
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
    float yHist = 400;
    sf::Text tHist;
    tHist.setFont(font);
    tHist.setCharacterSize(14);
    tHist.setFillColor(sf::Color::White);
    tHist.setStyle(sf::Text::Regular);
    tHist.setPosition(ANCHO_VENTANA - 500, yHist);
    tHist.setString("Historial:");
    window.draw(tHist);
    yHist += 28;
    for (const auto& linea : historial) {
        tHist.setString(linea);
        tHist.setPosition(ANCHO_VENTANA - 500, yHist);
        window.draw(tHist);
        yHist += 34;
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
                        ResultadoTurno resultado = juego->lanzarDadoYJugarTurno();
                        textoDado.setString("Dado: " + to_string(resultado.resultadoDado));
                        mostrarMensaje(resultado.descripcion);
                        agregarAlHistorial(resultado.descripcion);
                        
                        // Actualizar el texto del turno con información del estado
                        int actual = juego->obtenerJugadorActual();
                        string turnoText = "Turno: " + juego->obtenerJugador(actual).conseguirNombre();
                        
                        // Agregar información de estado del jugador
                        if (juego->obtenerJugador(actual).estaEnPozo()) {
                            turnoText += " (EN POZO)";
                        }
                        if (juego->obtenerJugador(actual).getTurnosPerdidos() > 0) {
                            turnoText += " (Pierde " + to_string(juego->obtenerJugador(actual).getTurnosPerdidos()) + " turnos)";
                        }
                        
                        textoJugadorActual.setString(turnoText);
                    }
                }
            }
        }
    }
}


void JuegoGUI::mostrarMensaje(const string& mensaje) {
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


void JuegoGUI::agregarAlHistorial(const string& accion) {
    historial.push_back(accion);
    if (historial.size() > 10) {
        historial.erase(historial.begin());
    }
}

// Métodos para ser llamado desde Juego (patrón Observer)
void JuegoGUI::actualizarTurno() {
    if (juegoIniciado && juego) {
        int actual = juego->obtenerJugadorActual();
        textoJugadorActual.setString("Turno: Jugador " + to_string(actual + 1));
    }
}

void JuegoGUI::actualizarMovimiento(int jugadorIndex, int posicionAnterior, int nuevaPosicion) {
    if (juegoIniciado && juego) {
        string movimiento = "Jugador " + to_string(jugadorIndex + 1) + 
                           " se mueve de " + to_string(posicionAnterior) + 
                           " a " + to_string(nuevaPosicion);
        agregarAlHistorial(movimiento);
    }
}

void JuegoGUI::mostrarGanador(const string& nombreGanador) {
    if (juegoIniciado) {
        string mensajeGanador = "🎉 ¡" + nombreGanador + " HA GANADO! 🎉";
        mostrarMensaje(mensajeGanador);
        agregarAlHistorial(mensajeGanador);
    }
} 