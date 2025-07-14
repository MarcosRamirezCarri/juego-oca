# Juego de la Oca - Versión Gráfica

## 🎮 Descripción

Esta es la versión gráfica del Juego de la Oca, implementada usando SFML (Simple and Fast Multimedia Library). Proporciona una interfaz visual moderna y atractiva para jugar al clásico juego de mesa.

## ✨ Características

### Interfaz Visual
- **Tablero gráfico**: Visualización completa del tablero de 63 casillas
- **Casillas especiales**: Colores diferenciados para cada tipo de casilla
- **Fichas animadas**: Jugadores representados con fichas de colores
- **Interfaz intuitiva**: Botones y controles fáciles de usar

### Elementos Visuales
- **Casillas de Oca**: Doradas con etiqueta "OCA"
- **Casillas especiales**: Naranjas con etiquetas descriptivas
- **Meta**: Verde brillante con etiqueta "META"
- **Fichas de jugadores**: Colores diferenciados (Rojo, Azul, Verde, Amarillo)

### Controles
- **Ratón**: Clic en botones y elementos de la interfaz
- **Botón "LANZAR DADO"**: Para realizar movimientos
- **Ventana**: Cerrar para salir del juego

## 🚀 Instalación y Ejecución

### Opción 1: Script Automático (Recomendado)
```bash
cd gui
./ejecutar_gui.sh
```

Este script:
- Verifica si SFML está instalado
- Lo instala automáticamente si es necesario
- Compila el juego
- Lo ejecuta

### Opción 2: Instalación Manual

#### 1. Instalar SFML
```bash
sudo apt update
sudo apt install libsfml-dev
```

#### 2. Compilar el juego
```bash
cd gui
make clean
make
```

#### 3. Ejecutar
```bash
./juego_oca_gui
```

## 🎯 Cómo Jugar

1. **Inicio**: El juego se inicia automáticamente con 3 jugadores
2. **Turnos**: Los jugadores juegan en orden
3. **Lanzar dado**: Haz clic en "LANZAR DADO" para mover tu ficha
4. **Casillas especiales**: Se activan automáticamente al caer en ellas
5. **Victoria**: Llega exactamente a la casilla 63 (META)

## 🎨 Características Técnicas

### Tecnologías Utilizadas
- **SFML 2.5+**: Biblioteca multimedia para C++
- **C++17**: Características modernas del lenguaje
- **POO**: Diseño orientado a objetos completo

### Arquitectura
- **Separación de responsabilidades**: Lógica del juego separada de la interfaz
- **Reutilización de código**: Usa las mismas clases del juego de consola
- **Extensibilidad**: Fácil agregar nuevas características visuales

### Rendimiento
- **60 FPS**: Fluidez garantizada
- **Optimización**: Renderizado eficiente
- **Memoria**: Gestión automática con smart pointers

## 🔧 Personalización

### Cambiar Colores
Edita las constantes de color en `JuegoGUI.h`:
```cpp
sf::Color COLOR_FONDO = sf::Color(34, 139, 34); // Verde bosque
sf::Color COLOR_CASILLA_OCA = sf::Color(255, 215, 0); // Dorado
// ... más colores
```

### Agregar Jugadores
Modifica `main_gui.cpp`:
```cpp
std::vector<std::string> nombresJugadores = {"Ana", "Carlos", "María", "Pedro"};
```

### Cambiar Tamaño de Ventana
Edita las constantes en `JuegoGUI.h`:
```cpp
static const int ANCHO_VENTANA = 1200;
static const int ALTO_VENTANA = 800;
```

## 🐛 Solución de Problemas

### Error: "SFML no está instalado"
```bash
sudo apt install libsfml-dev
```

### Error: "No se pudo cargar la fuente"
El juego usará una fuente del sistema por defecto.

### Error de compilación
```bash
make clean
make
```

### Ventana no se abre
Verifica que tienes un entorno gráfico activo (X11, Wayland, etc.)

## 📁 Estructura de Archivos

```
gui/
├── JuegoGUI.h          # Cabecera de la interfaz gráfica
├── JuegoGUI.cpp        # Implementación de la interfaz
├── main_gui.cpp        # Programa principal gráfico
├── Makefile            # Compilación con SFML
├── ejecutar_gui.sh     # Script de instalación y ejecución
└── README_GUI.md       # Esta documentación
```

## 🎓 Uso Educativo

Esta versión gráfica es perfecta para:
- **Demostraciones**: Mostrar el juego en presentaciones
- **Aprendizaje**: Visualizar conceptos de POO
- **Evaluación**: Proyectos de programación avanzada
- **Investigación**: Experimentar con interfaces gráficas

## 🔮 Futuras Mejoras

- [ ] Animaciones de movimiento
- [ ] Efectos de sonido
- [ ] Configuración de jugadores interactiva
- [ ] Guardado/carga de partidas
- [ ] Modo multijugador en red
- [ ] Temas visuales personalizables

## 📞 Soporte

Para problemas o preguntas:
1. Revisa esta documentación
2. Verifica la instalación de SFML
3. Consulta los logs de compilación
4. Revisa la documentación técnica principal

---

**¡Disfruta jugando al Juego de la Oca con gráficos!** 🎮 