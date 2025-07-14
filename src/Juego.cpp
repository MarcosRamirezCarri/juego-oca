#include "Juego.h"
#include <iostream>
#include <limits>
#include <algorithm>

Juego::Juego(const std::vector<std::string>& nombresJugadores) 
    : cantidadJugadores(nombresJugadores.size()), 
      finDelJuego(false), 
      jugadorActual(0), 
      turnoExtra(false) {
    
    // Crear jugadores
    for (const auto& nombre : nombresJugadores) {
        jugadores.emplace_back(nombre);
    }
    
    // Crear dado y tablero - esto es para POO
    dado = std::make_unique<Dado>();
    tablero = std::make_unique<Tablero>();
    
    std::cout << "Juego creado con " << cantidadJugadores << " jugadores" << std::endl;
}

void Juego::iniciarJuego() {
    std::cout << "\n=== ¡COMIENZA EL JUEGO DE LA OCA! ===" << std::endl;
    
    while (!finDelJuego) {
        mostrarEstadoJuego();
        
        if (jugadores[jugadorActual].puedeJugar()) {
            jugarTurno();
        } else {
            std::cout << jugadores[jugadorActual].conseguirNombre() << " no puede jugar este turno." << std::endl;
            if (jugadores[jugadorActual].estaEnPozo()) {
                std::cout << "Está atrapado en el pozo." << std::endl;
            } else {
                std::cout << "Pierde " << jugadores[jugadorActual].getTurnosPerdidos() << " turnos." << std::endl;
                jugadores[jugadorActual].reducirTurnosPerdidos();
            }
            pasarTurno();
        }
        
        if (verificarGanador()) {
            finDelJuego = true;
            std::cout << "\n🎉 ¡" << jugadores[jugadorActual].conseguirNombre() << " HA GANADO! 🎉" << std::endl;
        }
    }
}

void Juego::jugarTurno() {
    Jugador& jugador = jugadores[jugadorActual];
    
    std::cout << "\n--- Turno de " << jugador.conseguirNombre() << " ---" << std::endl;
    std::cout << "Presiona Enter para lanzar el dado...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    int resultadoDado = dado->lanzar(); // lanzar dado
    std::cout << "Has sacado un " << resultadoDado << std::endl;
    
    int posicionActual = jugador.conseguirPosicion();
    int nuevaPosicion = posicionActual + resultadoDado;
    
    // Verificar si se pasa de la meta
    if (nuevaPosicion > 63) {
        int exceso = nuevaPosicion - 63;
        nuevaPosicion = 63 - exceso;
        std::cout << "Te has pasado de la meta. Retrocedes " << exceso << " casillas." << std::endl;
    }
    
    procesarMovimiento(jugadorActual, nuevaPosicion);
    
    // Verificar si es casilla de oca para turno extra
    Casilla* casilla = tablero->obtenerCasilla(nuevaPosicion);
    if (casilla && casilla->getNombre() == "Oca") {
        turnoExtra = true;
        std::cout << "¡Turno extra por caer en oca!" << std::endl;
    } else {
        turnoExtra = false;
        pasarTurno();
    }
}

bool Juego::verificarGanador() const {
    return jugadores[jugadorActual].conseguirPosicion() == 63;
}

void Juego::mostrarEstadoJuego() const {
    std::cout << "\n=== ESTADO DEL JUEGO ===" << std::endl;
    for (size_t i = 0; i < jugadores.size(); i++) {
        const Jugador& jugador = jugadores[i];
        std::cout << (i == jugadorActual ? "→ " : "  ") 
                  << jugador.conseguirNombre() 
                  << ": Casilla " << jugador.conseguirPosicion();
        
        if (jugador.estaEnPozo()) {
            std::cout << " (EN POZO)";
        }
        if (jugador.getTurnosPerdidos() > 0) {
            std::cout << " (Pierde " << jugador.getTurnosPerdidos() << " turnos)";
        }
        std::cout << std::endl;
    }
    std::cout << "========================" << std::endl;
}

void Juego::procesarMovimiento(int jugadorIndex, int nuevaPosicion) {
    Jugador& jugador = jugadores[jugadorIndex];
    int posicionAnterior = jugador.conseguirPosicion();
    
    // Verificar si hay otro jugador en la nueva posición (excepto en el pozo)
    if (nuevaPosicion != 31) { // El pozo permite múltiples jugadores
        for (size_t i = 0; i < jugadores.size(); i++) {
            if (i != jugadorIndex && jugadores[i].conseguirPosicion() == nuevaPosicion) {
                std::cout << "¡Hay otro jugador en esa casilla! No puedes moverte." << std::endl;
                return;
            }
        }
    }
    
    // Mover al jugador
    jugador.moverJugador(nuevaPosicion);
    std::cout << jugador.conseguirNombre() << " se mueve de la casilla " 
              << posicionAnterior << " a la casilla " << nuevaPosicion << std::endl;
    
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
            std::cout << jugadores[i].conseguirNombre() << " ha sido liberado del pozo." << std::endl;
        }
    }
}

bool Juego::hayJugadoresEnPozo() const {
    return std::any_of(jugadores.begin(), jugadores.end(), 
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
std::pair<int, std::string> Juego::lanzarDadoYJugarTurno() {
    int resultado = dado->lanzar();
    int posInicial = jugadores[jugadorActual].conseguirPosicion();
    int nuevaPos = posInicial + resultado;
    // Rebote si se pasa de la meta
    if (nuevaPos > 63) {
        int exceso = nuevaPos - 63;
        nuevaPos = 63 - exceso;
    }
    std::string movimiento = jugadores[jugadorActual].conseguirNombre() +
        " se mueve de la casilla " + std::to_string(posInicial) +
        " a la casilla " + std::to_string(nuevaPos);
    procesarMovimiento(jugadorActual, nuevaPos);
    // Obtener descripción de la casilla
    Casilla* casilla = tablero->obtenerCasilla(jugadores[jugadorActual].conseguirPosicion());
    std::string desc = casilla ? casilla->getDescripcion() : "";
    if (!desc.empty()) {
        movimiento += "\n" + desc;
    }
    return {resultado, movimiento};
} 