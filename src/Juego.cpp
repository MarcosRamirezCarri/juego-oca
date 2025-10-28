#include "Juego.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <random>
#include <unordered_set>

using namespace std;

Juego::Juego(const vector<string>& nombresJugadores, int meta, bool especialesAleatorios) 
    : cantidadJugadores(nombresJugadores.size()), 
      finDelJuego(false), 
      jugadorActual(0), 
      turnoExtra(false),
      gui(nullptr),
      meta(meta),
      especialesAleatorios(especialesAleatorios),
      posicionPozo(31) {
    
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
       
        // Verificar si el jugador puede jugar
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
        
        // Verificar si el jugador ha ganado
        if (verificarGanador()) {
            finDelJuego = true;
            cout << "\n🎉 ¡" << jugadores[jugadorActual].conseguirNombre() << " HA GANADO! 🎉" << endl;
        }
    }
}

void Juego::reiniciarJuego() {
    finDelJuego = false;
    jugadorActual = 0;
    turnoExtra = false;

    // Reiniciar completamente el estado de todos los jugadores
    for (auto& jugador : jugadores) {
        jugador.moverJugador(0);  // Posición inicial
        jugador.perderTurnos(-jugador.getTurnosPerdidos());  // Resetear turnos perdidos
        if (jugador.estaEnPozo()) {
            jugador.salirPozo();  // Sacar del pozo si está atrapado
        }
    }

    // Reiniciar el dado
    dado = make_unique<Dado>();

    // Reiniciar las casillas
    inicializarCasillas();
}

void Juego::jugarTurno() {
    Jugador& jugador = jugadores[jugadorActual];
    
    
    
    int resultadoDado = dado->lanzar(); // lanzar dado
    
    int posicionActual = jugador.conseguirPosicion();
    int nuevaPosicion = posicionActual + resultadoDado;
    
    // Verificar si se pasa de la meta
    if (nuevaPosicion > meta) {
        int exceso = nuevaPosicion - meta;
        nuevaPosicion = meta - exceso;
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
    return jugadores[jugadorActual].conseguirPosicion() == meta;
}

void Juego::procesarMovimiento(int jugadorIndex, int nuevaPosicion) {
    Jugador& jugador = jugadores[jugadorIndex];
    int posicionAnterior = jugador.conseguirPosicion();
    
    // Verificar si hay otro jugador en la nueva posición (excepto en el pozo)
    if (nuevaPosicion != posicionPozo) { // El pozo permite múltiples jugadores
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
    if (nuevaPosicion == posicionPozo) {
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
    if (nuevaPos > meta) {
        int exceso = nuevaPos - meta;
        nuevaPos = meta - exceso;
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
    
    // Verificar si es casilla de oca para turno extra 
    if (casilla && casilla->getNombre() == "Oca") {
        turnoExtra = true;
        // No agregar el mensaje del turno extra al movimiento para el historial
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

// Método para conectar con la interfaz gráfica
void Juego::setGUI(JuegoGUI* interfaz) {
    gui = interfaz;
}

// Métodos privados para manejar las casillas
void Juego::inicializarCasillas() {
    // Normalizar meta
    if (meta < 10) meta = 63;
    casillas.clear();
    casillas.resize(meta + 1); // 0..meta (0 no se usa por lógica)

    // Inicializar todas las casillas como normales
    for (int i = 1; i <= meta; i++) {
        casillas[i] = make_unique<CasillaNormal>(i);
    }

    // Siempre colocar la meta
    casillas[meta] = make_unique<CasillaJardin>(meta);

    // Configurar casillas especiales
    if (!especialesAleatorios) {
        // Puente 6 -> 12
        if (6 <= meta && 12 <= meta) casillas[6] = make_unique<CasillaPuente>(6, 12);
        // Ocas clásicas cada 9 empezando en 9, hasta <= meta
        vector<int> ocas;
        for (int o = 9; o < meta; o += 9) {
            if (o <= meta) ocas.push_back(o);
        }
        for (size_t idx = 0; idx < ocas.size(); ++idx) {
            int from = ocas[idx];
            int to = (idx + 1 < ocas.size()) ? ocas[idx + 1] : meta;
            if (from <= meta && to <= meta) casillas[from] = make_unique<CasillaOca>(from, to);
        }
        // Posadas 12 y 19
        if (12 <= meta) casillas[12] = make_unique<CasillaPosada>(12, 1);
        if (19 <= meta) casillas[19] = make_unique<CasillaPosada>(19, 1);
        // Laberinto 42 -> 30 (solo si ambos existen)
        if (42 <= meta && 30 <= meta) casillas[42] = make_unique<CasillaLaberinto>(42, 30);
        // Pozo 31
        if (31 <= meta) { casillas[31] = make_unique<CasillaPozo>(31); posicionPozo = 31; }
        else { posicionPozo = -1; }
        // Carcel 56
        if (56 <= meta) casillas[56] = make_unique<CasillaCarcel>(56, 2);
        // Calavera 58 -> 1
        if (58 <= meta) casillas[58] = make_unique<CasillaCalavera>(58, 1);
    } else {
        // Generación aleatoria de casillas especiales
        random_device rd;
        mt19937 gen(rd());
        auto pickUnique = [&](int lo, int hi, int count, unordered_set<int>& used){
            uniform_int_distribution<> d(lo, hi);
            vector<int> result;
            int guard = 0;
            while ((int)result.size() < count && guard < 10000) {
                ++guard;
                int v = d(gen);
                if (v <= 1 || v >= meta) continue; // evitar inicio/meta
                if (used.count(v)) continue;
                used.insert(v);
                result.push_back(v);
            }
            sort(result.begin(), result.end());
            return result;
        };

        unordered_set<int> usados;
        // Ocas: cada 9 (9, 18, 27, ...), última oca salta a meta
        vector<int> posOcas;
        for (int o = 9; o < meta; o += 9) {
            posOcas.push_back(o);
            usados.insert(o);
        }
        for (size_t i = 0; i < posOcas.size(); ++i) {
            int from = posOcas[i];
            int to = (i + 1 < posOcas.size()) ? posOcas[i + 1] : meta;
            casillas[from] = make_unique<CasillaOca>(from, to);
        }

        // Puente: 1 par
        {
            uniform_int_distribution<> dStart(2, max(2, meta - 6));
            int start = dStart(gen);
            while (usados.count(start)) start = dStart(gen);
            usados.insert(start);
            uniform_int_distribution<> dEnd(start + 2, meta - 1);
            int end = dEnd(gen);
            while (usados.count(end)) { if (end + 1 < meta) ++end; else break; }
            casillas[start] = make_unique<CasillaPuente>(start, end);
        }

        // Posadas: 2 posiciones
        vector<int> posPosadas = pickUnique(2, meta - 1, 2, usados);
        for (int p : posPosadas) casillas[p] = make_unique<CasillaPosada>(p, 1);

        // Pozo: 1 posicion
        {
            vector<int> v = pickUnique(2, meta - 1, 1, usados);
            if (!v.empty()) { casillas[v[0]] = make_unique<CasillaPozo>(v[0]); posicionPozo = v[0]; }
            else { posicionPozo = -1; }
        }

        // Laberinto: 1 posicion con destino hacia atrás
        {
            vector<int> v = pickUnique(5, meta - 1, 1, usados);
            if (!v.empty()) {
                int from = v[0];
                int back = max(1, from - max(5, meta / 5));
                casillas[from] = make_unique<CasillaLaberinto>(from, back);
            }
        }

        // Carcel: 1 posicion
        {
            vector<int> v = pickUnique(2, meta - 1, 1, usados);
            if (!v.empty()) casillas[v[0]] = make_unique<CasillaCarcel>(v[0], 2);
        }

        // Calavera: 1 posicion que te manda a 1
        {
            vector<int> v = pickUnique(2, meta - 1, 1, usados);
            if (!v.empty()) casillas[v[0]] = make_unique<CasillaCalavera>(v[0], 1);
        }
    }
}

Casilla* Juego::obtenerCasilla(int numero) const {
    if (numero >= 1 && numero <= meta) {
        return casillas[numero].get();
    }
    return nullptr;
} 

string Juego::obtenerNombreCasilla(int numero) const {
    if (numero >= 1 && numero <= meta) {
        const Casilla* c = casillas[numero].get();
        return c ? c->getNombre() : string("");
    }
    return "";
}