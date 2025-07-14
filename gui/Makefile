CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = juego_oca_gui
SOURCES = main_gui.cpp JuegoGUI.cpp ../src/Dado.cpp ../src/Jugador.cpp ../src/Casilla.cpp ../src/Tablero.cpp ../src/Juego.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Incluir directorios de cabeceras
INCLUDES = -I../assets -I.

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: ../src/%.cpp
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