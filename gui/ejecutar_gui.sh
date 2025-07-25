#!/bin/bash
echo "=== JUEGO DE LA OCA - VERSION GRAFICA (LINUX) ==="
echo


echo
echo "Compilando el juego..."
make clean
make

if [ $? -eq 0 ]; then
    echo "Compilacion exitosa!"
    echo
    echo "Iniciando el juego..."
    echo "Controles:"
    echo "  - Haz clic en 'LANZAR DADO' para jugar"
    echo "  - Cierra la ventana para salir"
    echo
    ./juego_oca_gui
else
    echo "Error en la compilacion."
    exit 1
fi 