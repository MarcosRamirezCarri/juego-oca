#!/bin/bash
echo "=== JUEGO DE LA OCA - VERSION GRAFICA (LINUX) ==="
echo

echo "Verificando si SFML esta instalado..."
if ! command -v sfml-config &> /dev/null
then
    echo "SFML no esta instalado."
    echo "Por favor instala SFML con tu gestor de paquetes o desde https://www.sfml-dev.org/download.php"
    exit 1
else
    echo "SFML encontrado."
fi

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