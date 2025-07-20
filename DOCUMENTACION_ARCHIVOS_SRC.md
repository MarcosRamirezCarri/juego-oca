# Documentación de Archivos - Carpeta `src/`

## Resumen General

La carpeta `src/` contiene toda la lógica del juego "Juego de la Oca" implementada en C++. Cada archivo tiene una responsabilidad específica y sigue principios de programación orientada a objetos para mantener el código organizado y mantenible.


### **Integración**
- Utiliza la clase `Juego` para toda la lógica del juego. Se conecta con  `Jugador`, `Dado` y `Casilla`. Toda esta logica se implementa al juego realmente mediante juegoGui, el cual asigna una interfaz a cada elemento. Luego en el archivo habra una explicacion de el GUI

---
# Explicacion de las clases:

## `Dado.cpp` - Generador de Números Aleatorios

### **Propósito**
Implementa la funcionalidad del dado del juego, generando números aleatorios del 1 al 6 para determinar el movimiento de los jugadores.

### **Estructura del Código**
```cpp
class Dado {
public:
    Dado();           // Constructor que inicializa el generador
    int lanzar();     // Retorna un número aleatorio entre 1-6
private:
 // Caras de dado
};
```

### **Integración**
- Utilizado por la clase `Juego` para determinar el movimiento de los jugadores
- No depende de otras clases del proyecto
- Sigue el principio de responsabilidad única

## `Jugador.cpp` - Gestión de Jugadores

### **Propósito**
Representa a un jugador individual en el juego, manteniendo su estado, posición y características específicas.

### **Funcionalidades Principales**
- **Gestión de posición**: Controla en qué casilla está el jugador
- **Estado del jugador**: Maneja si está en el pozo o perdiendo turnos
- **Información personal**: Almacena el nombre del jugador
- **Métodos de consulta**: Proporciona acceso a la información del jugador

### **Estructura del Código**
```cpp
class Jugador {
private:
    string nombre;
    int posicion;
    bool enPozo;
    int turnosPerdidos;
    
public:
    // Constructores y métodos de acceso
    // Métodos para modificar estado
    // Métodos para consultar información
};
```

### **Integración**
- Utilizado por `Juego` para manejar múltiples jugadores
- Interactúa con `Casilla` cuando el jugador cae en casillas especiales
- Proporciona información a la GUI para mostrar el estado de los jugadores

---

## `Casilla.cpp` - Casillas Especiales del Tablero

### **Propósito**
Implementa el sistema de casillas especiales del juego, cada una con comportamientos únicos que afectan a los jugadores que caen en ellas.

### **Funcionalidades Principales**
- **Sistema de herencia**: Utiliza polimorfismo para diferentes tipos de casillas.
- **Casillas especiales**: Oca, Puente, Posada, Pozo, Laberinto, Cárcel, Dados, Jardín, Calavera
- **Efectos dinámicos**: Cada casilla tiene su propia lógica de acción
- **Interacción con jugadores**: Modifica el estado de los jugadores según la casilla

### **Tipos de Casillas Implementadas**

#### **CasillaNormal**
- No tiene efecto especial
- Base para otras casillas

#### **CasillaOca**
- Avanza el jugador a la siguiente oca
- Otorga un turno extra

#### **CasillaPuente**
- Mueve al jugador a la casilla 12
- Otorga un turno extra

#### **CasillaPosada**
- El jugador pierde 1 turno

#### **CasillaPozo**
- El jugador queda atrapado hasta que otro caiga
- Libera al jugador anterior cuando alguien más cae

#### **CasillaLaberinto**
- Mueve al jugador a la casilla 30
- Pierde 1 turno

#### **CasillaCarcel**
- El jugador pierde 3 turnos

#### **CasillaDados**
- Mueve al jugador a la casilla 26 o 53
- Otorga un turno extra

#### **CasillaJardin**
- El jugador gana automáticamente

#### **CasillaCalavera**
- El jugador vuelve al inicio (casilla 1)

### **Estructura del Código**
```cpp
class Casilla {
public:
    virtual void accionJugador(Jugador& jugador) = 0;
    virtual string obtenerDescripcion() const = 0;
    virtual ~Casilla() = default;
};

// Implementaciones específicas para cada tipo de casilla
```

## 🎮 `Juego.cpp` - Lógica Principal del Juego

### **Propósito**
Clase central que coordina toda la lógica del juego, manejando turnos, movimientos, reglas y la interacción entre todos los componentes.

### **Funcionalidades Principales**
- **Gestión de turnos**: Controla qué jugador juega y cuándo
- **Lógica de movimiento**: Maneja el lanzamiento del dado y el movimiento de jugadores
- **Efectos de casillas**: Procesa las consecuencias de caer en casillas especiales
- **Estado del juego**: Mantiene información sobre si el juego está activo
- **Integración con GUI**: Es la clase que conecta todas las demas para interactuar con la GUI
- **Detección de ganador**: Identifica cuando un jugador gana el juego

### **Estructura del Código**
```cpp
class Juego {
private:
    vector<Jugador> jugadores;
    Tablero tablero;
    Dado dado;
    int jugadorActual;
    bool jugando;
    JuegoGUI* gui;  // Para integración con GUI
    
public:
    // Constructores y configuración
    // Métodos de juego principal
    // Métodos de consulta de estado
    // Métodos de integración con GUI
};
```

### **Métodos Principales**

#### **Gestión del Juego**
- `Juego(vector<string> nombres)`: Constructor que inicializa el juego
- `bool estaJugando() const`: Verifica si el juego está activo
- `void terminarJuego()`: Finaliza el juego

#### **Lógica de Turnos**
- `lanzarDadoYJugarTurno()`: Ejecuta un turno completo
- `void procesarMovimiento(int jugadorIndex, int nuevaPosicion)`: Maneja el movimiento
- `void liberarJugadoresDelPozo(int jugadorQueCayo)`: Libera jugadores del pozo

#### **Integración con GUI**
- `void setGUI(JuegoGUI* gui)`: Conecta el juego con la interfaz gráfica
- `void notificarMovimiento(int jugadorIndex, int posicionAnterior, int nuevaPosicion)`: Notifica cambios a la GUI
- `void notificarGanador(const string& nombreGanador)`: Notifica cuando hay un ganador

#### **Consultas de Estado**
- `int obtenerJugadorActual() const`: Retorna el índice del jugador actual
- `const Jugador& obtenerJugador(int index) const`: Obtiene información de un jugador
- `void mostrarEstadoJuego() const`: Muestra el estado actual del juego

### **Flujo de un Turno**
1. **Lanzar dado**: Se genera un número aleatorio del 1 al 6
2. **Calcular nueva posición**: Se suma el resultado del dado a la posición actual
3. **Verificar límites**: Si supera 63, rebota hacia atrás
4. **Aplicar efectos de casilla**: Se ejecuta la acción de la casilla destino
5. **Verificar ganador**: Si llega exactamente a 63, gana
6. **Actualizar turno**: Se pasa al siguiente jugador (si no hay turno extra)

### **Integración**
- **Coordinador central**: Utiliza todas las demás clases del proyecto
- **Patrón Observer**: Se conecta con la GUI para notificar cambios
- **Gestión de memoria**: Utiliza smart pointers para las casillas del tablero

---

## Estructura entre los Archivos

```
mainGUI.cpp
|   ↓ (usa)
JuegoGUI.cpp
|   ↓ (observa)
Juego.cpp
|  ↓ (usa)
├── Casilla.cpp (polimórfico)
├── Jugador.cpp
└── Dado.cpp
```

### **Jerarquía de Dependencias**
2. **`Juego.cpp`** - Depende de todas las demás clases
4. **`Casilla.cpp`** - Depende de `Jugador.cpp`
5. **`Jugador.cpp`** - No depende de otras clases del proyecto
6. **`Dado.cpp`** - No depende de otras clases del proyecto

---

## Principios de Diseño Aplicados

### **¿Donde se aplicaron los principios Programación Orientada a Objetos?**
- **Encapsulación**: Cada clase oculta sus datos internos
- **Herencia**: `Casilla` es la clase base para diferentes tipos de casillas
- **Polimorfismo**: Diferentes tipos de casillas tienen comportamientos únicos
- **Composición**: `Juego` contiene instancias de otras clases 

### **Detalles del desarrollo**
- **Observador**: `Juego` notifica cambios a `JuegoGUI`
- **Estrategia**: Diferentes tipos de casillas implementan diferentes estrategias
- **Memoria**: No se utiliza manejos de memoria manual, se utilizan dependencias internas de c++ para realizar una gestion automatica de la memoria.

### **Características de las dependencias de C++ Utilizadas**
- `auto` para deducción de tipos
- `std::unique_ptr` para gestión automática de memoria
- Range-based for loops para iteración simplificada

### **Compatibilidad**
- **Cross-platform**: Funciona en Linux y Windows
- **Dependencias**: Solo requiere la biblioteca estándar de C++ y SFML para hacer correr el GUI


Esta documentación proporciona una comprensión completa de cómo cada archivo contribuye al funcionamiento del juego y cómo se relacionan entre sí para crear una experiencia de juego cohesiva y bien estructurada. 