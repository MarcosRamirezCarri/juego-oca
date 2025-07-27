# Trabajo Practico Anual "El juego de la OCA"
# Entrega N° 2: INTERFAZ JUGABLE
### Realizado por Valentino Pocai y Marcos Ramirez


## Estructura del Proyecto

```
juego-oca/
├── assets/                     # Archivos de cabecera (.h)
│   ├── Casilla.h              # Definición de casillas especiales
│   ├── Dado.h                 # Definición de la clase Dado
│   ├── Juego.h                # Definición de la clase principal Juego
│   └── Jugador.h              # Definición de la clase Jugador
│
├── src/                       # Archivos de implementación (.cpp)
│   ├── Casilla.cpp            # Implementación de casillas especiales
│   ├── Dado.cpp               # Implementación del dado
│   ├── Juego.cpp              # Lógica principal del juego
│   └── Jugador.cpp            # Implementación de jugadores
│
├── gui/                       # Interfaz gráfica
│   ├── main_gui.cpp           # Punto de entrada del programa
│   ├── JuegoGUI.cpp           # Implementación de la interfaz gráfica
│   ├── JuegoGUI.h             # Definición de la interfaz gráfica
│   ├── Makefile               # Configuración de compilación
│   ├── ejecutar_gui.bat       # Script de ejecución para Windows
│   └── ejecutar_gui.sh        # Script de ejecución para Linux
│
├── README.md                  # Este archivo
├── DOCUMENTACION_ARCHIVOS_SRC.md  # Documentación sobre ./src detallada
└── DOCUMENTACION_TECNICA.md   # Documentación técnica general
```
---
# Juego de la Oca - Instrucciones para Windows

Esta guía te ayudará a preparar tu PC con Windows para compilar y ejecutar el Juego de la Oca con interfaz gráfica, incluso si no tienes experiencia previa con herramientas de desarrollo.

## 1. Requisitos previos

Necesitarás instalar:
- **MinGW-w64** (compilador de C++)
- **SFML** (librería gráfica)
- **Make** (herramienta de compilación)
- (Opcional) **Chocolatey** (gestor de paquetes para Windows)

## 2. Instalación paso a paso

### 2.1. Instalar MinGW-w64

1. Descarga el instalador desde: https://www.mingw-w64.org/downloads/ (
   adjunto video para una mejor guia https://www.youtube.com/watch?v=ucg7o5t-K7o
)
2. Durante la instalación, selecciona:
   - Architecture: `x86_64`
   - Threads: `posix`
   - Exception: `seh`
   - Build revision: la más reciente
3. Instala en una ruta sencilla, por ejemplo: `C:\mingw-w64`
4. Accede a las variables de entorno del sistema y **Agrega la carpeta `bin` al PATH**:
   - Ejemplo: `C:\mingw-w64\bin`
   - Busca "Editar las variables de entorno del sistema" en el menú inicio, edita la variable `PATH` (o "Path") y agrega la ruta.
5. Tambien puede ser visual studio para compilador de c++

### 2.2. Instalar SFML

1. Descarga SFML para GCC desde: https://www.sfml-dev.org/download/sfml/2.5.1/ (En caso de usar visual estudio como compilador de c++ descargar la version de visual studio para tu pc)
2. Extrae el contenido en `C:\SFML` (debe quedar con subcarpetas como `bin`, `lib`, `include`...)
3. **Agrega `C:\SFML\bin` al PATH** (igual que antes).

### 2.3. Instalar Make con Chocolatey (fácil y recomendado)

1. Si no tienes Chocolatey, instálalo siguiendo las instrucciones en https://chocolatey.org/install (Adjunto un video de como hacer la instalacion: https://www.youtube.com/watch?v=V4RXLCgsjT0)
2. Abre PowerShell como administrador y ejecuta:
   ```
   choco install make
   ```
3. Cierra y abre una nueva terminal. Verifica:
   ```
   make --version
   ```
   Debe mostrar la versión de Make.

---

## 3. Compilar y ejecutar el juego

1. Abre una terminal (PowerShell, CMD o la terminal de Visual Studio Code).
2. Navega a la carpeta `gui` del proyecto. Por ejemplo:
   ```
   cd "C:\Users\TU_USUARIO\Desktop\Marcos\juego-oca\gui"
   ```
3. Ejecuta:
   ```
   make clean
   make
   ```
4. Si la compilación es exitosa, ejecuta el juego:
   ```
   ./juego_oca_gui
   ```

---

## 4. Uso del script automático

También puedes usar el script `ejecutar_gui.bat`:

1. En la carpeta `gui`, ejecuta:
   ```
   .\ejecutar_gui.bat
   ```
2. El script compilará y ejecutará el juego automáticamente.

---

## 5. Problemas comunes y soluciones

- **'make' no se reconoce como comando:**
  - Asegúrate de haber instalado Make con Chocolatey y de abrir una nueva terminal tras la instalación.
  - Verifica que la ruta de Make esté en el PATH.

- **'g++' no se reconoce como comando:**
  - Asegúrate de que MinGW-w64 esté instalado y su carpeta `bin` esté en el PATH.

- **El juego pide DLLs de SFML al ejecutar:**
  - Asegúrate de que `C:\SFML\bin` esté en el PATH, o copia las DLLs de esa carpeta al mismo directorio donde está el `.exe`.

- **Errores de compilación relacionados con SFML:**
  - Asegúrate de haber descargado la versión de SFML para GCC compatible con tu versión de MinGW-w64.

---

## 6. Notas adicionales

- Si tienes espacios en las rutas, usa comillas dobles en los comandos de la terminal.
- Si tienes problemas con permisos, ejecuta la terminal como administrador.
- Si prefieres, puedes compilar manualmente con:
  ```
  g++ -std=c++17 -Wall -I../assets -I. main_gui.cpp JuegoGUI.cpp ../src/Dado.cpp ../src/Jugador.cpp ../src/Casilla.cpp ../src/Juego.cpp -o juego_oca_gui.exe -IC:\SFML\include -LC:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system
  ```

---

## 7. ¿Necesitas ayuda?

Si tienes problemas: Contacte con los alumnos responsables de la entrega:
Adjunto correo para enviar sus problemas en caso de surgir: gellollo@outlook.com.

### Trabajo Practico Anual realizado por Valentino Pocai y Marcos Ramirez.