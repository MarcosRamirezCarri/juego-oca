# Documentación de Archivos - Carpeta `src/`

## Resumen General

La carpeta `src/` contiene toda la lógica del juego "Juego de la Oca" implementada en C++. Cada archivo tiene una responsabilidad específica y sigue principios de programación orientada a objetos para mantener el código organizado y mantenible.


### **Integración**
- Utiliza la clase `Juego` para toda la lógica del juego. Se conecta con `Jugador`, `Dado` y `Casilla`. La interfaz gráfica se implementa con Qt en `qt/MainWindow.{h,cpp}` usando `QGraphicsView/QGraphicsScene` para dibujar el tablero y gestionar el flujo del juego (ver sección de GUI al final).

---
# Explicacion de las clases:

## `Dado.cpp` - Generador de Números Aleatorios

- Genera números del 1 al 6 (o la cantidad de caras configurada).
- API: `Dado(int caras = 6)`, `int lanzar()`.
- Usado por `Juego` para mover a los jugadores.

## `Jugador.cpp` - Gestión de Jugadores

- Modela nombre, posición, estado de pozo y turnos perdidos.
- Métodos clave: `moverJugador`, `perderTurnos`, `reducirTurnosPerdidos`, `entrarPozo`, `salirPozo`, `puedeJugar`, `conseguirNombre`, `conseguirPosicion`, `getTurnosPerdidos`, `estaEnPozo`.
- La GUI lee su estado a través de `Juego`.

---

## `Casilla.cpp` - Casillas del Tablero

- Implementa casillas con polimorfismo: cada tipo define su efecto.
- Tipos incluidos: `Oca`, `Puente`, `Posada`, `Pozo`, `Laberinto`, `Cárcel`, `Calavera` y `Jardín` (meta).
- La casilla `Oca` otorga turno extra (lo maneja `Juego`). `Jardín` es la meta; la victoria se valida en `Juego`.

```cpp
class Casilla {
public:
    virtual void accionJugador(Jugador& jugador) = 0;
    virtual string getDescripcion() const = 0;
    virtual ~Casilla() = default;
};
```

## `Juego.cpp` - Lógica Principal del Juego

Gestor principal del juego: turnos, movimientos, efectos de casillas y estado general.

### **Funcionalidades Principales**
- **Gestión de turnos**: Controla qué jugador juega y cuándo
- **Lógica de movimiento**: Maneja el lanzamiento del dado y el movimiento de jugadores
- **Efectos de casillas**: Procesa las consecuencias de caer en casillas especiales
- **Estado del juego**: Mantiene información sobre si el juego está activo
- **Integración con GUI**: Es la clase que conecta todas las demas para interactuar con la GUI
- **Detección de ganador**: Identifica cuando un jugador gana el juego
- **Reinicio del juego**: Regresa todos los jugadores a la posicion 0 reiniciando el mismo

### **Estructura del Código**
```cpp
class Juego {
private:
    vector<Jugador> jugadores;
    vector<unique_ptr<Casilla>> casillas; // 1..meta
    unique_ptr<Dado> dado;
    int cantidadJugadores;
    int jugadorActual;
    bool finDelJuego;
    bool turnoExtra;
    int meta;                   // última casilla del tablero (63–90)
    bool especialesAleatorios;  // si se generan casillas especiales al azar
    int posicionPozo;           // posición del pozo (si existe)

    void inicializarCasillas();
    Casilla* obtenerCasilla(int numero) const;

public:
    Juego(const vector<string>& nombresJugadores,
          int meta = 63,
          bool especialesAleatorios = false);

    // Bucle y flujo de juego
    void jugarTurno();
    ResultadoTurno lanzarDadoYJugarTurno();
    bool verificarGanador() const;
    void procesarMovimiento(int jugadorIndex, int nuevaPosicion);
    void liberarJugadoresDelPozo(int jugadorQueCayo);
    void pasarTurno();

    // Consultas para la GUI
    const Jugador& obtenerJugador(int index) const;
    int obtenerCantidadJugadores() const;
    int obtenerJugadorActual() const;
    bool estaJugando() const;
    int obtenerMeta() const;
    string obtenerNombreCasilla(int numero) const;

    // Reinicio completo
    void reiniciarJuego();
};
```

### **Métodos Principales**

#### **Gestión del juego**
- `Juego(vector<string> nombres, int meta, bool especialesAleatorios)`
- `bool estaJugando() const`
- `void reiniciarJuego()`

#### **Lógica de turnos**
- `ResultadoTurno lanzarDadoYJugarTurno()`
- `void jugarTurno()`
- `void procesarMovimiento(int jugadorIndex, int nuevaPosicion)`
- `void liberarJugadoresDelPozo(int jugadorQueCayo)`
- `void pasarTurno()`
- `bool verificarGanador() const`

#### **Consultas de estado**
- `int obtenerJugadorActual() const`
- `const Jugador& obtenerJugador(int index) const`
- `int obtenerMeta() const`
- `string obtenerNombreCasilla(int numero) const`

#### **Integración con GUI**
- La GUI consulta el estado directamente. `setGUI(JuegoGUI*)` existe pero no es necesario en el flujo actual.

### **Reinicio del juego**
- Todos vuelven a la casilla 0, se limpian estados (pozo y turnos perdidos), se reinician dado y casillas.

### **Flujo de un Turno**
1. **Lanzar dado**: Se genera un número aleatorio del 1 al 6
2. **Calcular nueva posición**: Se suma el resultado del dado a la posición actual
3. **Verificar límites**: Si supera la meta, rebota hacia atrás (reflejo)
4. **Aplicar efectos de casilla**: Se ejecuta la acción de la casilla destino (pozo permite múltiples jugadores)
5. **Verificar ganador**: Si llega exactamente a la meta, gana
6. **Actualizar turno**: Se pasa al siguiente jugador (si no hay turno extra por Oca)

---

## Estructura entre los Archivos

```
qt/MainWindow.cpp  (Qt GUI)
|   ↓ (usa)
Juego.cpp          (lógica)
|  ↓ (usa)
├── Casilla.cpp    (polimórfico)
├── Jugador.cpp
└── Dado.cpp
```

### **Jerarquía de Dependencias**
- `Juego.cpp` depende de `Jugador`, `Casilla` y `Dado`.
- `Casilla.cpp` depende de `Jugador`.
- `Jugador.cpp` y `Dado.cpp` no dependen del resto.

---

## Principios de Diseño Aplicados

### **¿Donde se aplicaron los principios Programación Orientada a Objetos?**
- Encapsulación en todas las clases.
- Herencia y polimorfismo en `Casilla` y sus derivadas.
- Composición en `Juego`.

### **Detalles del desarrollo**
- Gestión de memoria con `std::unique_ptr` y RAII.

### **Características de las dependencias de C++ Utilizadas**
- `auto` para deducción de tipos y `std::unique_ptr` para gestión automática de memoria

### **Compatibilidad**
- Funciona en Linux y Windows y requiere la biblioteca estándar de C++ y Qt para la interfaz gráfica


Esta documentación proporciona una comprensión completa de cómo cada archivo contribuye al funcionamiento del juego y cómo se relacionan entre sí para crear una experiencia de juego cohesiva y bien estructurada. 