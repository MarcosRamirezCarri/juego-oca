#include "JuegoGUI.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

JuegoGUI::JuegoGUI() 
    : window(sf::VideoMode(ANCHO_VENTANA, ALTO_VENTANA), "Juego de la Oca - POO"),
      juegoIniciado(false),
      jugadorSeleccionado(0),
      cantidadJugadoresSeleccionada(2),
      enConfiguracion(true) {
    
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
    
    // Inicializar elementos de UI de texto
    configurarTexto(tituloJuego, "JUEGO DE LA OCA", 40, sf::Color::White, 
                   ANCHO_VENTANA / 2 - 200, 10);
    
    configurarTexto(textoJugadorActual, "", 28, sf::Color::White, ANCHO_VENTANA - 1450, 160);
    
    configurarTexto(textoDado, "", 24, sf::Color::White, ANCHO_VENTANA - 1000, 110);
    
    configurarTexto(textoMensaje, "", 18, sf::Color::Yellow, ANCHO_VENTANA - 600, 150);

    configurarTexto(textoAutores, "Creado por: Valentino Pocai y Marcos Ramirez", 20, sf::Color::White, 
                   ANCHO_VENTANA / 2 - 200, 860);
    
    // Botón del dado
    botonDado.setSize(sf::Vector2f(140, 60));
    botonDado.setFillColor(sf::Color(70, 130, 180));
    botonDado.setPosition(ANCHO_VENTANA - 1000, 160);

    // Botón de reiniciar juego
    botonReiniciarJuego.setSize(sf::Vector2f(160, 60));
    botonReiniciarJuego.setFillColor(COLOR_BOTON_REINICIAR);
    botonReiniciarJuego.setPosition(ANCHO_VENTANA - 200, 10);
    
    configurarTexto(textoBotonReiniciar, "REINICIAR JUEGO", 16, sf::Color::White, 0, 0);
    textoBotonReiniciar.setPosition(
        botonReiniciarJuego.getPosition().x + botonReiniciarJuego.getSize().x / 2 - textoBotonReiniciar.getGlobalBounds().width / 2,
        botonReiniciarJuego.getPosition().y + botonReiniciarJuego.getSize().y / 2 - textoBotonReiniciar.getGlobalBounds().height / 2
    );
    
    
    // Texto del boton del dado
    configurarTexto(textoBotonDado, "LANZAR DADO", 16, sf::Color::White, 0, 0);
    // Centrar el texto en el botón
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
        
        configurarTexto(numerosCasillas[i], to_string(i), 12, COLOR_TEXTO, 0, 0);
        configurarTexto(nombresCasillas[i], "", 10, COLOR_TEXTO, 0, 0);
        
        // Posicionar casillas en patrón serpenteante
        sf::Vector2f pos = obtenerPosicionCasilla(i);
        casillas[i].setPosition(pos);
        numerosCasillas[i].setPosition(pos.x + 2, pos.y + 2);
        nombresCasillas[i].setPosition(pos.x + 2, pos.y + TAMANO_CASILLA - 12);
        
        // Colorear casillas especiales
        switch (i) {
            case 6:
                casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
                nombresCasillas[i].setString("PUENTE");
                break;
            case 19:
                casillas[i].setFillColor(COLOR_CASILLA_ESPECIAL);
                nombresCasillas[i].setString("POSADA");
                break;
            case 31:
                casillas[i].setFillColor(COLOR_CASILLA_POZO);
                nombresCasillas[i].setString("POZO");
                break;
            case 42:
                casillas[i].setFillColor(COLOR_CASILLA_LABERINTO);
                nombresCasillas[i].setString("LABERINTO");
                break;
            case 56:
                casillas[i].setFillColor(COLOR_CASILLA_CARCEL);
                nombresCasillas[i].setString("CARCEL");
                break;
            case 58:
                casillas[i].setFillColor(COLOR_CASILLA_CALAVERA);
                nombresCasillas[i].setString("CALAVERA");
                break;
            case 63:
                casillas[i].setFillColor(COLOR_CASILLA_META);
                nombresCasillas[i].setString("META");
                break;
            default:
                // Casillas de OCA (múltiplos de 9 hasta 54)
                if (i % 9 == 0 && i <= 54 && i > 0) {
                    casillas[i].setFillColor(COLOR_CASILLA_OCA);
                    nombresCasillas[i].setString("OCA");
                } else {
                    casillas[i].setFillColor(COLOR_CASILLA_NORMAL);
                }
                break;
        }
    }
    // Inicializar el texto del turno
    textoJugadorActual.setString("Turno: Esperando inicio del juego");
    
    // Inicializar pantalla de configuración
    inicializarConfiguracion();
}

void JuegoGUI::ejecutar() {
    while (window.isOpen()) {
        if (enConfiguracion) {
            procesarEventosConfiguracion();
            window.clear(COLOR_FONDO);
            dibujarConfiguracion();
        } else {
            procesarEventos();
            window.clear(COLOR_FONDO);
            dibujarTablero();
            dibujarJugadores();
            dibujarUI();
        }
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
    
    vector<sf::Color> coloresFichas = {
        sf::Color::Red,
        sf::Color::Blue,
        sf::Color::Green,
        sf::Color::Yellow
    };
    
    for (size_t i = 0; i < nombres.size(); i++) {
        // Ficha del jugador
        sf::CircleShape ficha(18);
        ficha.setFillColor(coloresFichas[i % coloresFichas.size()]);
        ficha.setOutlineThickness(1);
        
        ficha.setOutlineColor(sf::Color::Black);
        fichasJugadores.push_back(ficha);
        
        // Nombre del jugador
        sf::Text nombre;
        configurarTexto(nombre, nombres[i], 20, coloresFichas[i % coloresFichas.size()], 
                       ANCHO_VENTANA - 600, 250 + i * 30);
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
    window.draw(botonReiniciarJuego);
    window.draw(textoBotonReiniciar);
    window.draw(textoAutores);

    // Dibujar historial de acciones
    float yHist = 400;
    sf::Text tHist;
    tHist.setFont(font);
    tHist.setCharacterSize(18);
    tHist.setFillColor(sf::Color::White);
    tHist.setStyle(sf::Text::Regular);
    tHist.setPosition(ANCHO_VENTANA - 600, yHist);
    tHist.setString("Historial:");
    window.draw(tHist);
    yHist += 30;
    for (const auto& linea : historial) {
        tHist.setString(linea);
        tHist.setPosition(ANCHO_VENTANA - 600, yHist);
        window.draw(tHist);
        yHist += 38;
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
                // Verificar clic en botón de reiniciar juego
                if (botonReiniciarJuego.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                 reiniciarJuego();
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

    // Agregar espaciado entre casillas
    float x = MARGEN + columna * (TAMANO_CASILLA + ESPACIADO_CASILLAS);
    float y = ALTO_VENTANA - MARGEN - (fila + 1) * (TAMANO_CASILLA + ESPACIADO_CASILLAS);
    
    return sf::Vector2f(x, y);
}


void JuegoGUI::agregarAlHistorial(const string& accion) {
    historial.push_back(accion);
    if (historial.size() > 10) {
        historial.erase(historial.begin());
    }
}

void JuegoGUI::configurarTexto(sf::Text& texto, const string& contenido, int tamano, const sf::Color& color, float x, float y) {
    texto.setFont(font);
    texto.setString(contenido);
    texto.setCharacterSize(tamano);
    texto.setFillColor(color);
    texto.setPosition(x, y);
}

// Métodos de configuración
void JuegoGUI::inicializarConfiguracion() {
    // Título de configuración
    configurarTexto(textoConfiguracion, "CONFIGURACION DEL JUEGO", 40, sf::Color::White, 
                   ANCHO_VENTANA / 2 - 300, 10);
    
    // Texto de cantidad de jugadores
    configurarTexto(textoCantidadJugadores, "Cantidad de Jugadores: 2", 24, sf::Color::White, 
                   ANCHO_VENTANA / 2 - 150, 150);
    
    // Botones para cambiar cantidad de jugadores
    botonMenosJugadores.setSize(sf::Vector2f(50, 50));
    botonMenosJugadores.setFillColor(sf::Color(100, 100, 100));
    botonMenosJugadores.setPosition(ANCHO_VENTANA / 2 - 250, 140);
    
    botonMasJugadores.setSize(sf::Vector2f(50, 50));
    botonMasJugadores.setFillColor(sf::Color(100, 100, 100));
    botonMasJugadores.setPosition(ANCHO_VENTANA / 2 + 250, 140);
    
    configurarTexto(textoMenosJugadores, "-", 30, sf::Color::White, 
                   ANCHO_VENTANA / 2 - 230, 145);
    configurarTexto(textoMasJugadores, "+", 30, sf::Color::White, 
                   ANCHO_VENTANA / 2 + 260, 145);
    
    // Botón iniciar juego
    botonIniciarJuego.setSize(sf::Vector2f(200, 60));
    botonIniciarJuego.setFillColor(sf::Color(70, 130, 180));
    botonIniciarJuego.setPosition(ANCHO_VENTANA / 2 - 100, 400);
    
    configurarTexto(textoIniciarJuego, "INICIAR JUEGO", 20, sf::Color::White, 
                   ANCHO_VENTANA / 2 - 80, 415);
    
    // Inicializar campos de nombres
    actualizarCamposNombres();
}

void JuegoGUI::actualizarCamposNombres() {
    camposNombres.clear();
    rectangulosNombres.clear();
    
    for (int i = 0; i < cantidadJugadoresSeleccionada; i++) {
        // Rectángulo para el campo de texto
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(200, 30));
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2);
        rect.setPosition(ANCHO_VENTANA / 2 - 100, 220 + i * 40);
        rectangulosNombres.push_back(rect);
        
        // Texto del campo
        sf::Text campo;
        string nombrePorDefecto = "Jugador " + to_string(i + 1);
        configurarTexto(campo, nombrePorDefecto, 16, sf::Color::Black, 
                       ANCHO_VENTANA / 2 - 95, 225 + i * 40);
        camposNombres.push_back(campo);
    }
}

void JuegoGUI::dibujarConfiguracion() {
    // Dibujar título
    window.draw(textoConfiguracion);
    
    // Dibujar cantidad de jugadores
    window.draw(textoCantidadJugadores);
    window.draw(botonMenosJugadores);
    window.draw(botonMasJugadores);
    window.draw(textoMenosJugadores);
    window.draw(textoMasJugadores);
    
    // Dibujar campos de nombres
    for (const auto& rect : rectangulosNombres) {
        window.draw(rect);
    }
    for (const auto& campo : camposNombres) {
        window.draw(campo);
    }
    
    // Dibujar botón iniciar
    window.draw(botonIniciarJuego);
    window.draw(textoIniciarJuego);
}

void JuegoGUI::procesarEventosConfiguracion() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Verificar clic en botón menos jugadores
                if (botonMenosJugadores.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cambiarCantidadJugadores(-1);
                }
                
                // Verificar clic en botón más jugadores
                if (botonMasJugadores.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cambiarCantidadJugadores(1);
                }
                
                // Verificar clic en botón iniciar juego
                if (botonIniciarJuego.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    iniciarJuegoDesdeConfiguracion();
                }
            }
        }
    }
}

void JuegoGUI::cambiarCantidadJugadores(int cambio) {
    int nuevaCantidad = cantidadJugadoresSeleccionada + cambio;
    if (nuevaCantidad >= 2 && nuevaCantidad <= 4) {
        cantidadJugadoresSeleccionada = nuevaCantidad;
        textoCantidadJugadores.setString("Cantidad de Jugadores: " + to_string(cantidadJugadoresSeleccionada));
        actualizarCamposNombres();
    }
}

void JuegoGUI::iniciarJuegoDesdeConfiguracion() {
    // Recopilar nombres de jugadores
    vector<string> nombres;
    for (const auto& campo : camposNombres) {
        string nombre = campo.getString().toAnsiString();
        if (nombre.empty()) {
            nombre = "Jugador " + to_string(nombres.size() + 1);
        }
        nombres.push_back(nombre);
    }
    
    // Inicializar el juego
    inicializarJuego(nombres);
    enConfiguracion = false;
}

void JuegoGUI::reiniciarJuego() {
    // Limpiar el juego actual
    if (juego) {
        juego.reset();
    }
    
    // Limpiar vectores de jugadores
    fichasJugadores.clear();
    nombresJugadores.clear();
    historial.clear();
    
    // Reiniciar variables de estado
    juegoIniciado = false;
    enConfiguracion = true;
    cantidadJugadoresSeleccionada = 2;
    
    // Reinicializar la pantalla de configuración
    inicializarConfiguracion();
    
    // Actualizar el texto de cantidad de jugadores
    textoCantidadJugadores.setString("Cantidad de Jugadores: " + to_string(cantidadJugadoresSeleccionada));
}

 