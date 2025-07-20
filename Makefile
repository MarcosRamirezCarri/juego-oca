CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iassets
TARGET = juego_oca
SOURCES = src/main.cpp src/Dado.cpp src/Jugador.cpp src/Casilla.cpp src/Juego.cpp
OBJECTS = $(SOURCES:src/%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) 