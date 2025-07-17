# Juego de la Oca en C++

Un juego de mesa clásico implementado en C++ usando Programación Orientada a Objetos.

## Descripción

Este proyecto implementa el juego de la Oca con todas sus reglas tradicionales:
- Tablero de 63 casillas
- Casillas especiales con efectos únicos
- Sistema de turnos y penalizaciones
- **Interfaz unificada**: Una sola lógica para consola y gráfico

## Arquitectura Unificada

El proyecto ahora usa una **arquitectura unificada** donde:

- **`Juego`** es la clase principal que contiene toda la lógica del juego
- **`JuegoGUI`** es una interfaz gráfica que se conecta con `Juego` usando el patrón Observer
- **No hay duplicación** de lógica entre consola y gráfico
- **Una sola fuente de verdad** para las reglas del juego

### Ventajas de esta Arquitectura:

1. **Mantenimiento más fácil**: Los cambios en las reglas solo se hacen en `Juego`
2. **Consistencia**: Ambos modos (consola y gráfico) usan exactamente la misma lógica
3. **Extensibilidad**: Fácil agregar nuevas interfaces (web, móvil, etc.)
4. **Testing**: Se puede probar la lógica independientemente de la interfaz

## Casillas Especiales

- **Oca** (9, 18, 27, 36, 45, 54): Salta a la siguiente oca + turno extra
- **Puente** (6): Va directamente a la casilla 12
- **Posada** (19): Pierde 1 turno
- **Pozo** (31): Queda atrapado hasta que otro caiga ahí
- **Laberinto** (42): Retrocede a la casilla 30
- **Cárcel** (56): Pierde 2 turnos
- **Calavera** (58): Vuelve a la casilla 1
- **Jardín de la Oca** (63): Meta final

## Compilación

### Linux/macOS
Para compilar el proyecto:
```bash
make
```

Para ejecutar:
```bash
./juego_oca
```

O usar:
```bash
make run
```

### Windows
Ver `README_WINDOWS.md` para instrucciones detalladas.

**Versión de consola (recomendada):**
```cmd
ejecutar_windows.bat
```

**Versión gráfica:**
```cmd
cd gui
ejecutar_gui.bat
```

## Ejecución

### Versión de Consola
```bash
./juego_oca
```

### Versión Gráfica (Recomendada)
```bash
cd gui
./ejecutar_gui.sh
```

**Nota**: La versión gráfica requiere SFML. El script de instalación lo configurará automáticamente.

## Estructura del Proyecto

```
juego-oca/
├── src/              # Código fuente (.cpp)
│   ├── main.cpp      # Programa principal e interfaz
│   ├── Dado.cpp      # Implementación del dado
│   ├── Jugador.cpp   # Implementación de jugadores
│   ├── Casilla.cpp   # Implementación de casillas especiales
│   ├── Tablero.cpp   # Implementación del tablero
│   └── Juego.cpp     # Implementación del juego
├── assets/           # Archivos de cabecera (.h)
│   ├── Dado.h        # Clase para el dado
│   ├── Jugador.h     # Clase para los jugadores
│   ├── Casilla.h     # Clases para las casillas especiales
│   ├── Tablero.h     # Clase para el tablero
│   └── Juego.h       # Clase principal del juego
├── gui/              # Versión gráfica del juego
│   ├── JuegoGUI.h/.cpp # Interfaz gráfica con SFML
│   ├── main_gui.cpp  # Programa principal gráfico
│   ├── Makefile      # Compilación con SFML
│   ├── ejecutar_gui.sh # Script de instalación
│   └── README_GUI.md # Documentación gráfica
├── Makefile          # Archivo de compilación (consola)
├── README.md         # Este archivo
├── DOCUMENTACION_TECNICA.md # Documentación POO
└── ejemplo_juego.txt # Ejemplo de ejecución
```

## Características del Diseño

El proyecto usa Programación Orientada a Objetos:
- Herencia: Casillas especiales heredan de Casilla
- Polimorfismo: Cada casilla tiene su comportamiento
- Encapsulación: Datos privados con métodos públicos
- Composición: El juego contiene jugadores, dado y tablero

### Clases Principales
- **Juego**: Controla el flujo del juego y contiene toda la lógica
- **Jugador**: Maneja el estado de cada jugador
- **Dado**: Genera números aleatorios
- **Tablero**: Contiene las casillas
- **Casilla**: Clase base para casillas especiales
- **JuegoGUI**: Interfaz gráfica que se conecta con Juego

### Patrón Observer

La comunicación entre `Juego` y `JuegoGUI` se hace mediante el **patrón Observer**:

- `Juego` notifica a `JuegoGUI` sobre cambios importantes
- `JuegoGUI` se actualiza automáticamente cuando cambia el estado del juego
- Esto permite animaciones y actualizaciones en tiempo real
- La lógica del juego está completamente separada de la presentación

## Reglas del Juego

1. Los jugadores comienzan en la casilla 0
2. Se lanza un dado de 1-6 para avanzar
3. Las casillas especiales activan efectos inmediatamente
4. Para ganar, debes llegar exactamente a la casilla 63
5. Si te pasas de la meta, retrocedes desde la casilla 63
6. No pueden estar dos fichas en la misma casilla (excepto en el pozo)

## Autor

Desarrollado como trabajo práctico de Programación Orientada a Objetos. 