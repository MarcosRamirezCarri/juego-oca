# Documentación Técnica - Juego de la Oca

## Diseño Orientado a Objetos

### 1. Principios POO Aplicados

#### 1.1 Encapsulación
- Datos privados con métodos públicos para acceder y modificar
- Control de acceso a través de métodos públicos

#### 1.2 Herencia
- Casilla es la clase base abstracta
- Cada tipo de casilla hereda de Casilla
- Polimorfismo para tratar todas las casillas igual

#### 1.3 Polimorfismo
- accionJugador() se implementa diferente en cada subclase
- El comportamiento se determina en tiempo de ejecución

#### 1.4 Composición
- Juego contiene Jugadores, Dado y Tablero
- Tablero contiene vector de casillas
- Uso de unique_ptr para gestión de memoria

### 3. Análisis de Clases

#### 3.1 Clase Juego
**Responsabilidades:**
- Controlar el flujo del juego
- Gestionar turnos de jugadores
- Aplicar reglas del juego
- Verificar condiciones de victoria

**Atributos principales:**
- `vector<Jugador> jugadores`: Lista de jugadores
- `unique_ptr<Dado> dado`: Dado del juego
- `unique_ptr<Tablero> tablero`: Tablero con casillas
- `int jugadorActual`: Índice del jugador actual
- `bool finDelJuego`: Estado del juego

#### 3.2 Clase Jugador
**Responsabilidades:**
- Mantener estado individual del jugador
- Gestionar posición y penalizaciones
- Controlar si puede jugar

**Atributos principales:**
- `string nombre`: Nombre del jugador
- `int posicion`: Posición actual en el tablero
- `int turnosPerdidos`: Turnos que debe perder
- `bool enPozo`: Si está atrapado en el pozo

#### 3.3 Clase Casilla (Abstracta)
**Responsabilidades:**
- Definir interfaz común para todas las casillas
- Aplicar efectos específicos al jugador

**Métodos virtuales:**
- `accionJugador(Jugador&)`: Aplica el efecto de la casilla
- `getDescripcion()`: Retorna descripción del efecto

#### 3.4 Subclases de Casilla
**CasillaOca:**
- Salta a la siguiente casilla de oca
- Otorga turno extra

**CasillaPuente:**
- Mueve al jugador a la casilla 12

**CasillaPosada:**
- Hace perder 1 turno

**CasillaPozo:**
- Atrapa al jugador hasta que otro caiga

**CasillaLaberinto:**
- Retrocede al jugador a la casilla 30

**CasillaCarcel:**
- Hace perder 2 turnos

**CasillaCalavera:**
- Regresa al jugador a la casilla 1

**CasillaJardin:**
- Meta final del juego

### 4. Patrones de Diseño

#### 4.1 Strategy Pattern
- Cada tipo de casilla implementa una estrategia diferente
- El comportamiento se selecciona en tiempo de ejecución
- El tablero crea diferentes tipos de casillas según su número
- Los jugadores observan el estado del pozo para liberarse

### 5. Gestión de Memoria

- `std::unique_ptr` para dado y tablero
- `std::unique_ptr` para casillas en el vector del tablero
- Gestión automática de memoria sin memory leaks
- Los recursos se liberan automáticamente al salir del scope
- No hay necesidad de delete manual

### 6. Características de C++ Moderno

#### 6.1 C++17 Features
- `std::any_of` para algoritmos
- `auto` para deducción de tipos
- List initialization
- Range-based for loops

#### 6.2 Modern C++ Practices
- `#pragma once` en lugar de include guards
- `const` correctness
- References en lugar de punteros cuando es posible
- `std::string` en lugar de C-style strings

### 7. Interfaz de Usuario

#### 7.1 Consola Interactiva
- Menú principal con opciones
- Entrada de datos validada
- Salida formateada y clara
- Estados del juego visibles

#### 7.2 Experiencia de Usuario
- Mensajes informativos para cada acción
- Indicadores visuales del jugador actual
- Confirmación de acciones importantes

### 8. Extensibilidad

#### 8.1 Nuevas Casillas
- Fácil agregar nuevos tipos de casillas
- Solo requiere heredar de `Casilla` e implementar `accionJugador()`

#### 8.2 Nuevas Reglas
- Modificar la clase `Juego` para nuevas mecánicas
- Extender `Jugador` para nuevos estados

#### 8.3 Interfaz Gráfica
- Separación clara entre lógica e interfaz
- Fácil migración a GUI manteniendo las clases del modelo

### 9. Testing y Debugging

- Verificación de rangos de entrada
- Mensajes de error descriptivos

### 10. Conclusión

Este diseño demuestra una aplicación sólida de los principios de POO:
- **Modularidad**: Cada clase tiene responsabilidades bien definidas
- **Reutilización**: Las clases pueden extenderse fácilmente
- **Mantenibilidad**: Código claro y bien estructurado
- **Escalabilidad**: Fácil agregar nuevas características

El proyecto cumple con los requisitos académicos de POO y proporciona una base sólida para futuras extensiones.

## 11. Resumen Técnico

### 11.1 Características de C++17 Utilizadas

#### Justificación de `std::any_of`
```cpp
bool Juego::hayJugadoresEnPozo() const {
    return std::any_of(jugadores.begin(), jugadores.end(), 
                      [](const Jugador& jugador) { return jugador.estaEnPozo(); });
}
```
**Ventajas:**
- **Legibilidad**: Expresa claramente la intención de verificar si alguno cumple la condición
- **Eficiencia**: Se detiene en el primer elemento que cumple la condición
- **Seguridad**: Menos propenso a errores que un bucle manual
- **Estilo funcional**: Sigue paradigmas modernos de programación

#### Justificación de `auto` y Range-based for loops
```cpp
for (const auto& nombre : nombresJugadores) {
    jugadores.emplace_back(nombre);
}
```
**Ventajas:**
- **Menos verboso**: No requiere especificar tipos explícitamente
- **Mantenibilidad**: Si cambia el tipo de contenedor, el código sigue funcionando
- **Legibilidad**: Código más limpio y enfocado en la lógica
- **Prevención de errores**: Evita errores de índices y tipos

#### Justificación de `std::unique_ptr`
```cpp
std::unique_ptr<Dado> dado;
std::unique_ptr<Tablero> tablero;
```
**Ventajas:**
- **RAII**: Gestión automática de recursos (Resource Acquisition Is Initialization)
- **Prevención de memory leaks**: Liberación automática de memoria
- **Semántica de propiedad**: Indica claramente quién es dueño del recurso
- **Exception safety**: Garantiza liberación de recursos incluso con excepciones

#### Justificación de Structured Bindings
```cpp
auto [resultado, desc] = juego->lanzarDadoYJugarTurno();
```
**Ventajas:**
- **Legibilidad**: Acceso directo a elementos de tuplas/pairs
- **Menos verboso**: No requiere `.first` y `.second`
- **Nombres descriptivos**: Variables con nombres significativos
- **Prevención de errores**: No confusión entre elementos del pair

### 11.2 Comparación con Código Rudimentario

#### Ejemplo: Verificación de jugadores en pozo

**Código moderno (C++17):**
```cpp
bool Juego::hayJugadoresEnPozo() const {
    return std::any_of(jugadores.begin(), jugadores.end(), 
                      [](const Jugador& jugador) { return jugador.estaEnPozo(); });
}
```

**Código rudimentario:**
```cpp
bool Juego::hayJugadoresEnPozo() const {
    for (size_t i = 0; i < jugadores.size(); i++) {
        if (jugadores[i].estaEnPozo()) {
            return true;
        }
    }
    return false;
}
```

**Diferencias clave:**
- **Expresividad**: El código moderno expresa mejor la intención
- **Mantenibilidad**: Menos líneas de código, más fácil de mantener
- **Seguridad**: Menos propenso a errores de índices
- **Rendimiento**: Potencialmente más eficiente (optimizaciones del compilador)

### 11.3 Impacto en los Principios POO

#### Encapsulación
- `std::unique_ptr` encapsula la gestión de memoria
- Los smart pointers implementan RAII correctamente
- Las clases no necesitan manejar manualmente la liberación de recursos

#### Polimorfismo
- Los smart pointers manejan correctamente la herencia
- Evitan problemas de slicing y memory leaks
- Facilitan el uso seguro de polimorfismo

#### Reutilización
- Las características modernas hacen el código más modular
- Facilitan la extensión y modificación del proyecto
- Mejoran la interfaz entre clases

### 11.4 Beneficios Educativos

#### Para el Estudiante
- **Aprendizaje de C++ moderno**: Familiarización con características actuales
- **Mejores prácticas**: Uso de patrones recomendados por la industria
- **Pensamiento funcional**: Introducción a paradigmas modernos
- **Gestión de memoria**: Comprensión de RAII y smart pointers

#### Para el Profesor
- **Evaluación de conocimiento**: Demuestra comprensión de C++ moderno
- **Calidad del código**: Muestra atención a detalles y mejores prácticas
- **Preparación profesional**: Expone al estudiante a herramientas de la industria
- **Base para futuros proyectos**: Establece buenos hábitos de programación

### 11.5 Conclusión del Resumen

La utilización de características de C++17 en este proyecto no es solo una demostración de conocimiento técnico, sino una aplicación práctica de principios fundamentales de programación:

1. **Seguridad**: Prevención de errores comunes y memory leaks
2. **Legibilidad**: Código más expresivo y fácil de entender
3. **Mantenibilidad**: Facilita futuras modificaciones y extensiones
4. **Eficiencia**: Aprovecha optimizaciones del compilador moderno
5. **Profesionalismo**: Demuestra conocimiento de mejores prácticas actuales

Estas características complementan perfectamente los principios de POO, creando un proyecto que no solo cumple con los requisitos académicos, sino que también establece una base sólida para el desarrollo de software profesional. 