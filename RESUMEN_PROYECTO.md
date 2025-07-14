# Juego de la Oca - Proyecto Completo

## Resumen del Proyecto

Este proyecto implementa el Juego de la Oca en C++ usando Programación Orientada a Objetos, con dos versiones:

1. Versión de Consola - Interfaz de texto
2. Versión Gráfica - Interfaz visual con SFML

## 🎯 Objetivos Cumplidos

### Programación Orientada a Objetos
- Encapsulación: Datos privados con métodos públicos
- Herencia: Jerarquía de casillas especiales
- Polimorfismo: Comportamiento dinámico de casillas
- Composición: Juego contiene jugadores, dado y tablero

### Interfaz Interactiva
- Consola: Menú interactivo
- Gráfica: Ventana con tablero visual

### Funcionalidad Completa
- Todas las reglas del juego implementadas
- Casillas especiales con efectos únicos
- Sistema de turnos y penalizaciones
- Verificación de victoria

## 🏗️ Arquitectura del Proyecto

### Estructura de Carpetas
```
juego-oca/
├── src/                    # Código fuente (.cpp)
│   ├── main.cpp           # Programa principal consola
│   ├── Dado.cpp           # Implementación del dado
│   ├── Jugador.cpp        # Implementación de jugadores
│   ├── Casilla.cpp        # Implementación de casillas
│   ├── Tablero.cpp        # Implementación del tablero
│   └── Juego.cpp          # Implementación del juego
├── assets/                 # Archivos de cabecera (.h)
│   ├── Dado.h             # Clase del dado
│   ├── Jugador.h          # Clase de jugadores
│   ├── Casilla.h          # Clases de casillas
│   ├── Tablero.h          # Clase del tablero
│   └── Juego.h            # Clase principal del juego
├── gui/                    # Versión gráfica
│   ├── JuegoGUI.h/.cpp    # Interfaz gráfica SFML
│   ├── main_gui.cpp       # Programa principal gráfico
│   ├── Makefile           # Compilación con SFML
│   ├── ejecutar_gui.sh    # Script de instalación
│   └── README_GUI.md      # Documentación gráfica
├── Makefile               # Compilación versión consola
├── README.md              # Documentación principal
├── DOCUMENTACION_TECNICA.md # Análisis POO detallado
├── CAMBIOS_ESTRUCTURA.md  # Historial de cambios
└── ejemplo_juego.txt      # Ejemplo de ejecución
```

## 🎨 Características de la Versión Gráfica

### Interfaz Visual
- **Tablero gráfico**: 63 casillas visualmente diferenciadas
- **Casillas especiales**: Colores únicos para cada tipo
- **Fichas animadas**: Jugadores con colores diferenciados
- **Controles intuitivos**: Botones y clics de ratón

### Elementos Visuales
- **Casillas de Oca**: Doradas con etiqueta "OCA"
- **Casillas especiales**: Naranjas con etiquetas descriptivas
- **Meta**: Verde brillante con etiqueta "META"
- **Fichas**: Rojo, Azul, Verde, Amarillo

### Tecnologías
- **SFML 2.5+**: Biblioteca multimedia para C++
- **C++17**: Características modernas
- **POO**: Diseño orientado a objetos completo

## 🚀 Cómo Ejecutar

### Versión de Consola
```bash
make          # Compilar
./juego_oca   # Ejecutar
```

### Versión Gráfica (Recomendada)
```bash
cd gui
./ejecutar_gui.sh  # Instala SFML, compila y ejecuta
```

## 🎯 Casillas Especiales Implementadas

| Casilla | Tipo | Efecto |
|---------|------|--------|
| 9, 18, 27, 36, 45, 54 | Oca | Salta a siguiente oca + turno extra |
| 6 | Puente | Va directamente a casilla 12 |
| 19 | Posada | Pierde 1 turno |
| 31 | Pozo | Queda atrapado hasta que otro caiga |
| 42 | Laberinto | Retrocede a casilla 30 |
| 56 | Cárcel | Pierde 2 turnos |
| 58 | Calavera | Vuelve a casilla 1 |
| 63 | Jardín | Meta final |

## 🔧 Características Técnicas

### Código
- **C++17**: Características modernas del lenguaje
- **Smart Pointers**: Gestión automática de memoria
- **Const Correctness**: Seguridad de datos
- **Exception Handling**: Manejo robusto de errores

### Compilación
- **Makefiles**: Automatización de compilación
- **Flags de optimización**: -Wall -Wextra -g
- **Dependencias**: SFML para versión gráfica
- **Multiplataforma**: Compatible con Linux, Windows, macOS

### Documentación
- **README completo**: Instrucciones de uso
- **Documentación técnica**: Análisis POO detallado
- **Ejemplos**: Casos de uso y ejecución
- **Comentarios**: Código bien documentado

## 🎓 Valor Educativo

### Para Estudiantes
- **POO práctica**: Implementación real de conceptos
- **Arquitectura de software**: Diseño modular
- **Interfaces gráficas**: Programación visual
- **Gestión de proyectos**: Organización de código

### Para Profesores
- **Material didáctico**: Ejemplo completo de POO
- **Evaluación**: Proyecto listo para calificar
- **Extensibilidad**: Base para agregar características
- **Documentación**: Análisis técnico detallado

## 🔮 Posibles Extensiones

### Funcionalidades
- [ ] Animaciones de movimiento
- [ ] Efectos de sonido
- [ ] Configuración de jugadores interactiva
- [ ] Guardado/carga de partidas
- [ ] Modo multijugador en red
- [ ] Temas visuales personalizables

### Mejoras Técnicas
- [ ] Tests unitarios
- [ ] Integración continua
- [ ] Optimización de rendimiento
- [ ] Soporte para más plataformas

## 📊 Métricas del Proyecto

- **Líneas de código**: ~1,500 líneas
- **Clases**: 8 clases principales
- **Archivos**: 20+ archivos organizados
- **Documentación**: 4 archivos de documentación
- **Versiones**: 2 interfaces (consola + gráfica)

## 🏆 Logros Destacados

1. **Implementación completa** del juego de la Oca
2. **Diseño POO sólido** con principios bien aplicados
3. **Interfaz gráfica moderna** usando SFML
4. **Código bien organizado** y documentado
5. **Fácil de usar** con scripts de instalación
6. **Extensible** para futuras mejoras

## 🎉 Conclusión

Este proyecto demuestra una **implementación profesional** del Juego de la Oca, combinando:

- **Programación Orientada a Objetos** bien aplicada
- **Interfaces modernas** (consola y gráfica)
- **Código limpio** y bien documentado
- **Facilidad de uso** para estudiantes y profesores

Es un **ejemplo excelente** para trabajos prácticos de POO y una base sólida para futuras extensiones.

---

**¡El Juego de la Oca está listo para jugar!** 🎮✨ 