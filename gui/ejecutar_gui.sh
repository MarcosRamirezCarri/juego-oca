#!/bin/bash

echo "=== JUEGO DE LA OCA - VERSIÓN GRÁFICA ==="
echo ""

# Verificar si SFML está instalado
if ! pkg-config --exists sfml-all; then
    echo "❌ SFML no está instalado."
    echo "Instalando SFML..."
    sudo apt update
    sudo apt install -y libsfml-dev
    if [ $? -ne 0 ]; then
        echo "❌ Error al instalar SFML. Por favor instálalo manualmente:"
        echo "   sudo apt install libsfml-dev"
        exit 1
    fi
    echo "✅ SFML instalado correctamente."
else
    echo "✅ SFML ya está instalado."
fi

echo ""
echo "Compilando el juego..."

# Compilar el juego
make clean
make

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
    echo ""
    echo "🎮 Iniciando el juego..."
    echo "Controles:"
    echo "  - Haz clic en 'LANZAR DADO' para jugar"
    echo "  - Cierra la ventana para salir"
    echo ""
    ./juego_oca_gui
else
    echo "❌ Error en la compilación."
    exit 1
fi 