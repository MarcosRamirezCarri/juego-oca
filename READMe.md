# Trabajo Practico Anual "El juego de la OCA"

# Entrega N° 2: INTERFAZ JUGABLE

## Alumnos: Valentino Pocai y Marcos Ramirez

## Juego de la Oca - Guía Completa para Windows

## Índice
1. [Requisitos del Sistema](#requisitos-del-sistema)
2. [Instalación Paso a Paso](#instalación-paso-a-paso)
3. [Compilación y Ejecución](#compilación-y-ejecución)
4. [Solución de Problemas](#solución-de-problemas)
5. [Estructura del Proyecto](#estructura-del-proyecto)
6. [Preguntas Frecuentes](#preguntas-frecuentes)

---

## Requisitos del Sistema

### **Versión Gráfica**
- **Windows 7 o superior**
- **SFML 2.5+** para Windows
- **MinGW-w64** o **Visual Studio**
- **Tarjeta gráfica compatible con OpenGL**

---

## Instalación Paso a Paso

### **Paso 1: Instalar MinGW-w64**

#### **Opción A: MSYS2 (Recomendada)**
1. Ve a https://www.msys2.org/
2. Descarga el instalador de MSYS2
3. Ejecuta el instalador y sigue las instrucciones
4. Abre **MSYS2 MinGW x64** desde el menú inicio
5. Ejecuta estos comandos:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-make
   ```
6. Agrega `C:\msys64\mingw64\bin` al PATH del sistema

#### **Opción B: MinGW-w64 Directo**
1. Ve a https://www.mingw-w64.org/downloads/
2. Descarga la versión para Windows x86_64
3. Extrae en `C:\mingw64`
4. Agrega `C:\mingw64\bin` al PATH del sistema

#### **Opción C: Visual Studio**
1. Instala Visual Studio Community (gratis)
2. En el instalador, selecciona "Desarrollo de escritorio con C++"
3. Usa el Developer Command Prompt para compilar

### **Paso 2: Verificar la Instalación**
Abre **Command Prompt** y ejecuta:
```cmd
g++ --version
```
Deberías ver algo como:
```
g++ (MinGW-W64 8.1.0) 8.1.0
```

### **Paso 3: Instalar SFML (Solo para versión gráfica)**

#### **Método 1: Descarga Manual**
1. Ve a https://www.sfml-dev.org/download.php
2. Descarga la versión para tu compilador:
   - **MinGW**: `SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip`
   - **Visual Studio**: `SFML-2.5.1-windows-vc15-64-bit.zip`
3. Extrae en `C:\SFML`
4. Agrega `C:\SFML\bin` al PATH del sistema

#### **Método 2: Con MSYS2**
```bash
pacman -S mingw-w64-x86_64-sfml
```

### **Paso 4: Instalar Make (Opcional)**
```bash
# Si usas MSYS2
pacman -S mingw-w64-x86_64-make

# O descarga desde
# http://gnuwin32.sourceforge.net/packages/make.htm
```

---

## Compilación y Ejecución

### **Versión Gráfica**

#### **Opción 1: Script Automático (Más Fácil)**
```cmd
# Navega a la carpeta GUI
cd gui

# Ejecuta el script automático
ejecutar_gui.bat
```

#### **Opción 2: Compilación Manual**
```cmd
# Navega a la carpeta GUI
cd gui

# Compila con SFML (ajusta las rutas según tu instalación)
g++ -std=c++17 -Wall -I../assets -I. -I"C:/SFML/include" ^
    main_gui.cpp JuegoGUI.cpp ../src/*.cpp ^
    -L"C:/SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system ^
    -o juego_oca_gui.exe

# Ejecuta el juego gráfico
juego_oca_gui.exe
```

#### **Opción 3: Usando Makefile**
```cmd
# Navega a la carpeta GUI
cd gui

# Compila usando el Makefile
make -f Makefile.windows

# Ejecuta el juego
make -f Makefile.windows run
```

---

## Solución de Problemas

### **Error: "g++ no se reconoce como comando"**

**Causa**: MinGW-w64 no está en el PATH del sistema

**Solución**:
1. Verifica que MinGW-w64 esté instalado
2. Agrega la ruta al PATH:
   - **MSYS2**: `C:\msys64\mingw64\bin`
   - **MinGW directo**: `C:\mingw64\bin`
3. Reinicia el Command Prompt
4. Verifica con: `g++ --version`

### **Error: "SFML no encontrado"**

**Causa**: SFML no está instalado o no está en el PATH

**Solución**:
1. Verifica que SFML esté en `C:\SFML`
2. Agrega `C:\SFML\bin` al PATH
3. Verifica que los archivos DLL estén en `C:\SFML\bin`:
   - `sfml-graphics-2.dll`
   - `sfml-window-2.dll`
   - `sfml-system-2.dll`

### **Error: "No se pudo cargar la fuente"**

**Causa**: No se encuentra la fuente del sistema

**Solución**:
- El juego usará una fuente por defecto
- No afecta la funcionalidad del juego
- Puedes ignorar este mensaje

### **Error: "make no se reconoce"**

**Causa**: Make no está instalado

**Solución**:
1. Instala Make para Windows
2. O compila manualmente con g++
3. O usa los scripts `.bat` automáticos

### **Error: "LNK1104: no se puede abrir el archivo"**

**Causa**: Rutas incorrectas de SFML

**Solución**:
1. Verifica que SFML esté en `C:\SFML`
2. Ajusta las rutas en el comando de compilación:
   ```cmd
   -I"C:/SFML/include" -L"C:/SFML/lib"
   ```

### **Error: "The application was unable to start correctly"**

**Causa**: DLLs de SFML faltantes

**Solución**:
1. Copia las DLLs de `C:\SFML\bin` al directorio del ejecutable
2. O agrega `C:\SFML\bin` al PATH del sistema

---

## Estructura del Proyecto

```
juego-oca/
├── src/                    # Código fuente del juego         
│   ├── Juego.cpp             # Lógica principal del juego
│   ├── Jugador.cpp           # Gestión de jugadores
│   ├── Casilla.cpp           # Casillas especiales
│   └── Dado.cpp              # Lógica del dado
├── assets/                 # Archivos de cabecera (.h)
│   ├── Juego.h
│   ├── Jugador.h
│   ├── Casilla.h
│   └── Dado.h
├── gui/                    # Versión gráfica
│   ├── main_gui.cpp          # Punto de entrada (GUI)
│   ├── JuegoGUI.cpp          # Interfaz gráfica
│   ├── JuegoGUI.h
│   ├── Makefile              # Makefile para Linux/macOS
│   ├── ejecutar_gui.sh       # Script para Linux/macOS
│   └── ejecutar_gui.bat      # Script para Windows
├── Makefile               # Makefile principal (Linux/macOS)
├── DOCUMENTACION_ARCHIVOS_SRC.md   # Detalle de cada archivo en ./src
├── DOCUMENTACION_TECNICA.md      # Documentacion total del proyecto
└── README.md      # Esta guía
```

---

## Preguntas Frecuentes

### **Q: ¿Cuál versión debo usar?**
**A**: 
- **Solo existe la versión gráfica** con interfaz visual
- **SFML** es requerido para ejecutar el juego
- **Ideal para** usuarios que prefieren interfaces gráficas

### **Q: ¿Por qué no funciona el juego?**
**A**: 
1. Verifica que SFML esté instalado correctamente
2. Asegúrate de que las DLLs estén en el PATH
3. Comprueba que MinGW-w64 esté configurado

### **Q: ¿Puedo modificar el código?**
**A**: 
- Sí, el código está bien documentado
- Usa `g++ -std=c++17` para compilar
- Consulta la documentación técnica

### **Q: ¿Cómo agrego más jugadores?**
**A**: 
- Usa la interfaz gráfica que permite 2-4 jugadores
- Modifica el código en `gui/main_gui.cpp` para cambiar el límite

### **Q: ¿El juego funciona en Windows 10/11?**
**A**: 
- Sí, es compatible con Windows 7, 8, 10 y 11
- Usa MinGW-w64 para mejor compatibilidad

### **Q: ¿Puedo distribuir el juego?**
**A**: 
- Sí, es código abierto
- Incluye las DLLs de SFML si distribuyes la versión gráfica

---

## Consejos de Uso

### **Para Desarrolladores**
- Usa **Visual Studio Code** con extensiones de C++
- Configura **IntelliSense** para mejor autocompletado
- Usa **Git** para control de versiones

### **Para Estudiantes**
- Lee la documentación técnica
- Experimenta modificando las casillas especiales
- Prueba diferentes configuraciones de jugadores

### **Para Usuarios Finales**
- Usa los scripts `.bat` para ejecución fácil
- El juego tiene una interfaz gráfica intuitiva
- Guarda tus partidas favoritas

---

## Soporte

Si tienes problemas: Contacte con los alumnos responsables de la entrega

Trabajo Practico Anual: Valentino Pocai, Marcos Ramirez