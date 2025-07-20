CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = juego_oca_gui

# Fuentes de la GUI
GUI_SOURCES = main_gui.cpp JuegoGUI.cpp
GUI_OBJECTS = $(GUI_SOURCES:.cpp=.o)

# Fuentes del juego (desde src)
GAME_SOURCES = ../src/Dado.cpp ../src/Jugador.cpp ../src/Casilla.cpp ../src/Juego.cpp
GAME_OBJECTS = $(notdir $(GAME_SOURCES:.cpp=.o))

# Todos los objetos
OBJECTS = $(GUI_OBJECTS) $(GAME_OBJECTS)

# Incluir directorios de cabeceras
INCLUDES = -I../assets -I.

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_LIBS)

# Regla para archivos objeto de la GUI
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regla para archivos objeto del juego (creados en gui/)
Dado.o: ../src/Dado.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

Jugador.o: ../src/Jugador.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

Casilla.o: ../src/Casilla.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

Juego.o: ../src/Juego.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

# Verificar si SFML está instalado
check-sfml:
	@echo "Verificando instalación de SFML..."
	@pkg-config --exists sfml-all || (echo "SFML no está instalado. Ejecuta: sudo apt install libsfml-dev" && exit 1)
	@echo "SFML encontrado ✓"

# Instalar dependencias
install-deps:
	sudo apt update
	sudo apt install libsfml-dev

# Compilar con verificación de dependencias
build: check-sfml all 