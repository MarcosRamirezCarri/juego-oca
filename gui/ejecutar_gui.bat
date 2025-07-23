@echo off
REM === JUEGO DE LA OCA - VERSION GRAFICA (WINDOWS) ===
echo === JUEGO DE LA OCA - VERSION GRAFICA (WINDOWS) ===
echo.

REM Verifica que C:\SFML existe
if not exist "C:\SFML" (
    echo No se encontro la carpeta C:\SFML.
    echo Por favor instala SFML manualmente:
    echo 1. Descarga SFML desde https://www.sfml-dev.org/download.php
    echo 2. Extrae en C:\SFML
    echo 3. Agrega C:\SFML\bin al PATH
    pause
    exit /b 1
) else (
    echo SFML encontrado en C:\SFML.
)

echo.
echo Compilando el juego...
make clean
make

if %errorlevel% equ 0 (
    echo Compilacion exitosa!
    echo.
    echo Iniciando el juego...
    echo Controles:
    echo   - Haz clic en 'LANZAR DADO' para jugar
    echo   - Cierra la ventana para salir
    echo.
    juego_oca_gui.exe
) else (
    echo Error en la compilacion.
    pause
    exit /b 1
) 