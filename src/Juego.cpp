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
    
    // Crear dado y tablero 
    dado = make_unique<Dado>();
    tablero = make_unique<Tablero>();
    
    cout << "Juego creado con " << cantidadJugadores << " jugadores" << endl;
}

Juego::~Juego() {
    // El destructor se encarga de limpiar automáticamente
}

void Juego::iniciarJuego() {
    cout << "\n=== ¡COMIENZA EL JUEGO DE LA OCA! ===" << endl;
    
    while (!finDelJuego) {
        mostrarEstadoJuego();
        
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
    
    cout << "\n--- Turno de " << jugador.conseguirNombre() << " ---" << endl;
    cout << "Presiona Enter para lanzar el dado...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    int resultadoDado = dado->lanzar(); // lanzar dado
    cout << "Has sacado un " << resultadoDado << endl;
    
    int posicionActual = jugador.conseguirPosicion();
    int nuevaPosicion = posicionActual + resultadoDado;
    
    // Verificar si se pasa de la meta
    if (nuevaPosicion > 63) {
        int exceso = nuevaPosicion - 63;
        nuevaPosicion = 63 - exceso;
        cout << "Te has pasado de la meta. Retrocedes " << exceso << " casillas." << endl;
    }
    
    procesarMovimiento(jugadorActual, nuevaPosicion);
    
    // Verificar si es casilla de oca para turno extra
    Casilla* casilla = tablero->obtenerCasilla(nuevaPosicion);
    if (casilla && casilla->getNombre() == "Oca") {
        turnoExtra = true;
        cout << "¡Turno extra por caer en oca!" << endl;
    } else {
        turnoExtra = false;
        pasarTurno();
    }
}

bool Juego::verificarGanador() const {
    return jugadores[jugadorActual].conseguirPosicion() == 63;
}

void Juego::mostrarEstadoJuego() const {
    cout << "\n=== ESTADO DEL JUEGO ===" << endl;
    for (size_t i = 0; i < jugadores.size(); i++) {
        const Jugador& jugador = jugadores[i];
        cout << (i == jugadorActual ? "→ " : "  ") 
                  << jugador.conseguirNombre() 
                  << ": Casilla " << jugador.conseguirPosicion();
        
        if (jugador.estaEnPozo()) {
            cout << " (EN POZO)";
        }
        if (jugador.getTurnosPerdidos() > 0) {
            cout << " (Pierde " << jugador.getTurnosPerdidos() << " turnos)";
        }
        cout << endl;
    }
    cout << "========================" << endl;
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
    Casilla* casilla = tablero->obtenerCasilla(nuevaPosicion);
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
pair<int, string> Juego::lanzarDadoYJugarTurno() {
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
        return {0, mensaje};
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
    
    procesarMovimiento(jugadorActual, nuevaPos);
    
    // Obtener descripción de la casilla
    Casilla* casilla = tablero->obtenerCasilla(jugadores[jugadorActual].conseguirPosicion());
    string desc = casilla ? casilla->getDescripcion() : "";
    if (!desc.empty()) {
        movimiento += "\n" + desc;
    }
    
    // Verificar si es casilla de oca para turno extra
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
    
    return {resultado, movimiento};
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