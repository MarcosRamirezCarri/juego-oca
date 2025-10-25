cmake_minimum_required(VERSION 3.16)
project(juego_oca_qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar herramientas automáticas de Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Intentar con Qt6 y si no, con Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME}
    qt/main.cpp
    qt/MainWindow.cpp
    # Lógica existente del juego
    src/Dado.cpp
    src/Jugador.cpp
    src/Casilla.cpp
    src/Juego.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/assets
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

# En Linux/Unix, definir el RPATH para ejecución desde el build dir (opcional)
if(UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH_USE_ORIGIN ON
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH FALSE
    )
endif()


