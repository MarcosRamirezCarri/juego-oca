#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "Juego.h"

using namespace std;

void mostrarMenu() {
    cout << "\n=== JUEGO DE LA OCA ===" << endl;
    cout << "1. Iniciar nuevo juego" << endl;
    cout << "2. Ver reglas" << endl;
    cout << "3. Salir" << endl;
    cout << "Seleccione una opción: ";
}

void mostrarReglas() {
    cout << "\n=== REGLAS DEL JUEGO DE LA OCA ===" << endl;
    cout << "• El objetivo es llegar exactamente a la casilla 63 (Jardín de la Oca)" << endl;
    cout << "• Se lanza un dado de 1 a 6 para avanzar" << endl;
    cout << "• Casillas especiales:" << endl;
    cout << "  - Oca (9, 18, 27, 36, 45, 54): Salta a la siguiente oca + turno extra" << endl;
    cout << "  - Puente (6): Va directamente a la casilla 12" << endl;
    cout << "  - Posada (19): Pierde 1 turno" << endl;
    cout << "  - Pozo (31): Queda atrapado hasta que otro caiga ahí" << endl;
    cout << "  - Laberinto (42): Retrocede a la casilla 30" << endl;
    cout << "  - Cárcel (56): Pierde 2 turnos" << endl;
    cout << "  - Calavera (58): Vuelve a la casilla 1" << endl;
    cout << "  - Jardín de la Oca (63): Meta final" << endl;
    cout << "• Si te pasas de la casilla 63, retrocedes desde ahí" << endl;
    cout << "• No pueden estar dos fichas en la misma casilla (excepto en el pozo)" << endl;
}

int obtenerNumeroJugadores() {
    int numJugadores;
    do {
        cout << "Ingrese el número de jugadores (2-4): ";
        cin >> numJugadores;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Por favor ingrese un número válido." << endl;
            continue;
        }
    } while (numJugadores < 2 || numJugadores > 4);
    return numJugadores;
}

vector<string> obtenerNombresJugadores(int numJugadores) {
    vector<string> nombres;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    for (int i = 0; i < numJugadores; i++) {
        string nombre;
        cout << "Ingrese el nombre del jugador " << (i + 1) << ": ";
        getline(cin, nombre);
        if (nombre.empty()) {
            nombre = "Jugador " + to_string(i + 1);
        }
        nombres.push_back(nombre);
    }
    return nombres;
}

void mostrarTablero(const vector<Jugador>& jugadores) {
    cout << "\n=== ESTADO DEL TABLERO ===" << endl;
    for (size_t i = 0; i < jugadores.size(); i++) {
        cout << jugadores[i].conseguirNombre() << ": Casilla " << jugadores[i].conseguirPosicion();
        if (jugadores[i].estaEnPozo()) {
            cout << " (EN POZO)";
        }
        if (jugadores[i].getTurnosPerdidos() > 0) {
            cout << " (Pierde " << jugadores[i].getTurnosPerdidos() << " turnos)";
        }
        cout << endl;
    }
    cout << "==========================" << endl;
}

int main() {
    srand(time(0)); // Inicializar semilla para números aleatorios
    
    int opcion;
    Juego* juego = nullptr;
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                if (juego != nullptr) {
                    delete juego;
                }
                
                int numJugadores = obtenerNumeroJugadores();
                vector<string> nombres = obtenerNombresJugadores(numJugadores);
                
                juego = new Juego(nombres);
                juego->iniciarJuego();
                
                delete juego;
                juego = nullptr;
                break;
            }
            case 2:
                mostrarReglas();
                break;
            case 3:
                cout << "¡Gracias por jugar!" << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (opcion != 3);
    
    if (juego != nullptr) {
        delete juego;
    }
    
    return 0;
} 