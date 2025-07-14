@echo off
echo === JUEGO DE LA OCA - VERSION CONSOLA ===
echo.

echo Compilando el juego...
make -f Makefile.windows clean
make -f Makefile.windows

if %errorlevel% equ 0 (
    echo Compilacion exitosa!
    echo.
    echo Iniciando el juego...
    echo.
    juego_oca.exe
) else (
    echo Error en la compilacion.
    echo.
    echo Asegurate de tener instalado:
    echo - MinGW-w64 o Visual Studio
    echo - Make para Windows
    pause
    exit /b 1
) 