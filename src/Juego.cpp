#include "Juego.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

Juego::Juego(const vector<string>& nombresJugadores) 
    : cantidadJugadores(nombresJugadores.size()), 
      finDelJuego(false), 
      jugadorActual(0), 
      turnoExtra(false),
      gui(nullptr) {
    
    // Crear jugadores
    for (const auto& nombre : nombresJugadores) {
        jugadores.emplace_back(nombre);
    }
    
    // Crear dado e inicializar casillas
    dado = make_unique<Dado>();
    inicializarCasillas();
    
    
}

Juego::~Juego() {
    // El destructor se encarga de limpiar automáticamente
}

void Juego::iniciarJuego() {
    
    while (!finDelJuego) {
       
        
        if (jugadores[jugadorActual].puedeJugar()) {
            jugarTurno();
        } else {
            cout << jugadores[jugadorActual].conseguirNombre() << " no puede jugar este turno." << endl;
            if (jugadores[jugadorActual].estaEnPozo()) {
                cout << "Está atrapado en el pozo." << endl;
            } else {
                cout << "Pierde " << jugadores[jugadorActual].getTurnosPerdidos() << " turnos." << endl;
                jugadores[jugadorActual].reducirTurnosPerdidos();
            }
            pasarTurno();
        }
        
        if (verificarGanador()) {
            finDelJuego = true;
            cout << "\n🎉 ¡" << jugadores[jugadorActual].conseguirNombre() << " HA GANADO! 🎉" << endl;
        }
    }
}

void Juego::jugarTurno() {
    Jugador& jugador = jugadores[jugadorActual];
    
    
    
    int resultadoDado = dado->lanzar(); // lanzar dado
    
    int posicionActual = jugador.conseguirPosicion();
    int nuevaPosicion = posicionActual + resultadoDado;
    
    // Verificar si se pasa de la meta
    if (nuevaPosicion > 63) {
        int exceso = nuevaPosicion - 63;
        nuevaPosicion = 63 - exceso;
        
    }
    
    procesarMovimiento(jugadorActual, nuevaPosicion);
    
    // Verificar si es casilla de oca para turno extra
    Casilla* casilla = obtenerCasilla(nuevaPosicion);
    if (casilla && casilla->getNombre() == "Oca") {
        turnoExtra = true;
        
    } else {
        turnoExtra = false;
        pasarTurno();
    }
}

bool Juego::verificarGanador() const {
    return jugadores[jugadorActual].conseguirPosicion() == 63;
}

void Juego::procesarMovimiento(int jugadorIndex, int nuevaPosicion) {
    Jugador& jugador = jugadores[jugadorIndex];
    int posicionAnterior = jugador.conseguirPosicion();
    
    // Verificar si hay otro jugador en la nueva posición (excepto en el pozo)
    if (nuevaPosicion != 31) { // El pozo permite múltiples jugadores
        for (size_t i = 0; i < jugadores.size(); i++) {
            if (i != jugadorIndex && jugadores[i].conseguirPosicion() == nuevaPosicion) {
                cout << "¡Hay otro jugador en esa casilla! No puedes moverte." << endl;
                return;
            }
        }
    }
    
    // Mover al jugador
    jugador.moverJugador(nuevaPosicion);
    cout << jugador.conseguirNombre() << " se mueve de la casilla " 
              << posicionAnterior << " a la casilla " << nuevaPosicion << endl;
    
    // Aplicar efecto de la casilla
    Casilla* casilla = obtenerCasilla(nuevaPosicion);
    if (casilla) {
        casilla->accionJugador(jugador);
    }
    
    // Verificar si cayó en el pozo y liberar a otros jugadores
    if (nuevaPosicion == 31) {
        liberarJugadoresDelPozo(jugadorIndex);
    }
}

void Juego::liberarJugadoresDelPozo(int jugadorQueCayo) {
    for (size_t i = 0; i < jugadores.size(); i++) {
        if (i != jugadorQueCayo && jugadores[i].estaEnPozo()) {
            jugadores[i].salirPozo();
            cout << jugadores[i].conseguirNombre() << " ha sido liberado del pozo." << endl;
        }
    }
}

bool Juego::hayJugadoresEnPozo() const {
    return any_of(jugadores.begin(), jugadores.end(), 
                      [](const Jugador& jugador) { return jugador.estaEnPozo(); });
}

void Juego::pasarTurno() {
    if (!turnoExtra) {
        jugadorActual = (jugadorActual + 1) % cantidadJugadores;
    }
}

// Métodos para la interfaz gráfica
const Jugador& Juego::obtenerJugador(int index) const {
    if (index >= 0 && index < jugadores.size()) {
        return jugadores[index];
    }
    // Retornar el primer jugador como fallback
    return jugadores[0];
}

int Juego::obtenerCantidadJugadores() const {
    return cantidadJugadores;
}

int Juego::obtenerJugadorActual() const {
    return jugadorActual;
}

bool Juego::estaJugando() const {
    return !finDelJuego;
}

// Nuevo método para la GUI
ResultadoTurno Juego::lanzarDadoYJugarTurno() {
    // Verificar si el jugador puede jugar
    if (!jugadores[jugadorActual].puedeJugar()) {
        string mensaje = jugadores[jugadorActual].conseguirNombre() + " no puede jugar este turno.";
        if (jugadores[jugadorActual].estaEnPozo()) {
            mensaje += " Está atrapado en el pozo.";
        } else {
            mensaje += " Pierde " + to_string(jugadores[jugadorActual].getTurnosPerdidos()) + " turnos.";
            jugadores[jugadorActual].reducirTurnosPerdidos();
        }
        pasarTurno();
        return ResultadoTurno(0, mensaje);
    }
    
    int resultado = dado->lanzar();
    int posInicial = jugadores[jugadorActual].conseguirPosicion();
    int nuevaPos = posInicial + resultado;
    
    // Rebote si se pasa de la meta
    if (nuevaPos > 63) {
        int exceso = nuevaPos - 63;
        nuevaPos = 63 - exceso;
    }
    
    string movimiento = jugadores[jugadorActual].conseguirNombre() +
        " se mueve de la casilla " + to_string(posInicial) +
        " a la casilla " + to_string(nuevaPos);
    
    // Obtener descripción de la casilla ANTES del movimiento
    Casilla* casilla = obtenerCasilla(nuevaPos);
    string desc = casilla ? casilla->getDescripcion() : "";
    if (!desc.empty()) {
        movimiento += "\n" + desc;
    }
    
    procesarMovimiento(jugadorActual, nuevaPos);
    
    // Verificar si es casilla de oca para turno extra (usando la casilla original)
    if (casilla && casilla->getNombre() == "Oca") {
        turnoExtra = true;
        movimiento += "\n¡Turno extra por caer en oca!";
    } else {
        turnoExtra = false;
        pasarTurno();
    }
    
    // Verificar ganador
    if (verificarGanador()) {
        finDelJuego = true;
        movimiento += "\n🎉 ¡" + jugadores[jugadorActual].conseguirNombre() + " HA GANADO! 🎉";
    }
    
    return ResultadoTurno(resultado, movimiento);
}

// Métodos para conectar con la interfaz gráfica
void Juego::setGUI(JuegoGUI* interfaz) {
    gui = interfaz;
}

void Juego::notificarCambioTurno() {
    if (gui) {
        // La GUI se actualizará automáticamente
        // Este método permite notificaciones específicas si es necesario
    }
}

void Juego::notificarMovimiento(int jugadorIndex, int posicionAnterior, int nuevaPosicion) {
    if (gui) {
        // Notificar a la GUI sobre el movimiento
        // Esto permite animaciones o actualizaciones específicas
    }
}

void Juego::notificarGanador(const string& nombreGanador) {
    if (gui) {
        // Notificar a la GUI sobre el ganador
        // Esto permite mostrar una pantalla de victoria especial
    }
}

// Métodos privados para manejar las casillas
void Juego::inicializarCasillas() {
    casillas.resize(64); // Casillas del 1 al 63 (índice 0 no se usa)
    
    // Inicializar todas las casillas como normales
    for (int i = 1; i <= 63; i++) {
        casillas[i] = make_unique<CasillaNormal>(i);
    }
    
    // Configurar casillas especiales
    casillas[6] = make_unique<CasillaPuente>();
    casillas[9] = make_unique<CasillaOca>(9);
    casillas[12] = make_unique<CasillaPosada>(); // Posada en casilla 12
    casillas[18] = make_unique<CasillaOca>(18);
    casillas[19] = make_unique<CasillaPosada>(); // Posada en casilla 19
    casillas[27] = make_unique<CasillaOca>(27);
    casillas[30] = make_unique<CasillaNormal>(30); // Casilla normal (destino del laberinto)
    casillas[31] = make_unique<CasillaPozo>();
    casillas[36] = make_unique<CasillaOca>(36);
    casillas[42] = make_unique<CasillaLaberinto>();
    casillas[45] = make_unique<CasillaOca>(45);
    casillas[54] = make_unique<CasillaOca>(54);
    casillas[56] = make_unique<CasillaCarcel>();
    casillas[58] = make_unique<CasillaCalavera>();
    casillas[63] = make_unique<CasillaJardin>();
}

Casilla* Juego::obtenerCasilla(int numero) const {
    if (numero >= 1 && numero <= 63) {
        return casillas[numero].get();
    }
    return nullptr;
} 