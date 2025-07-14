# Juego de la Oca - Instrucciones para Windows

## Requisitos

### Para la versión de consola:
- **MinGW-w64** o **Visual Studio** (compilador C++)
- **Make para Windows** (opcional, para usar los scripts)

### Para la versión gráfica:
- **SFML 2.5+** para Windows
- **MinGW-w64** o **Visual Studio**

## Instalación

### 1. Instalar MinGW-w64
1. Descarga MinGW-w64 desde: https://www.mingw-w64.org/downloads/
2. O usa MSYS2: https://www.msys2.org/
3. Agrega MinGW-w64 al PATH del sistema

### 2. Instalar SFML (solo para versión gráfica)
1. Ve a https://www.sfml-dev.org/download.php
2. Descarga la versión para tu compilador (MinGW o MSVC)
3. Extrae en `C:\SFML`
4. Agrega `C:\SFML\bin` al PATH del sistema

### 3. Instalar Make (opcional)
- Descarga Make para Windows desde: http://gnuwin32.sourceforge.net/packages/make.htm
- O usa el que viene con MinGW-w64

## Compilación y Ejecución

### Versión de Consola (Recomendada)

**Opción 1: Usando el script automático**
```cmd
ejecutar_windows.bat
```

**Opción 2: Compilación manual**
```cmd
g++ -std=c++17 -Wall -Iassets src/*.cpp -o juego_oca.exe
juego_oca.exe
```

**Opción 3: Usando Makefile**
```cmd
make -f Makefile.windows
make -f Makefile.windows run
```

### Versión Gráfica

**Opción 1: Usando el script automático**
```cmd
cd gui
ejecutar_gui.bat
```

**Opción 2: Compilación manual**
```cmd
cd gui
g++ -std=c++17 -Wall -I../assets -I. -I"C:/SFML/include" main_gui.cpp JuegoGUI.cpp ../src/*.cpp -L"C:/SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system -o juego_oca_gui.exe
juego_oca_gui.exe
```

**Opción 3: Usando Makefile**
```cmd
cd gui
make -f Makefile.windows
make -f Makefile.windows run
```

## Solución de Problemas

### Error: "g++ no se reconoce como comando"
- Instala MinGW-w64 y agrégalo al PATH
- O usa Visual Studio con el Developer Command Prompt

### Error: "No se pudo cargar la fuente"
- El juego usará una fuente por defecto
- No afecta la funcionalidad

### Error: "SFML no encontrado"
- Verifica que SFML esté instalado en `C:\SFML`
- Asegúrate de que `C:\SFML\bin` esté en el PATH

### Error: "make no se reconoce"
- Instala Make para Windows
- O compila manualmente con g++

## Estructura de Archivos para Windows

```
juego-oca/
├── src/                    # Código fuente
├── assets/                 # Archivos de cabecera
├── gui/                    # Versión gráfica
├── Makefile.windows        # Makefile para Windows
├── ejecutar_windows.bat    # Script para consola
├── gui/Makefile.windows    # Makefile GUI para Windows
└── gui/ejecutar_gui.bat    # Script para GUI
```

## Notas Importantes

- La versión de consola funciona sin dependencias externas
- La versión gráfica requiere SFML instalado
- Los archivos .exe se crean en el directorio raíz
- Usa MinGW-w64 para mejor compatibilidad 