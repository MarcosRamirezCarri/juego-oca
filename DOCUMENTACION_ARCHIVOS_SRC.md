# Estructura de archivos e importaciones

## src/

- `src/Juego.cpp`
  - Importaciones:
    - `"Juego.h"`
    - `<fstream>`
    - `<iostream>`
    - `<limits>`
    - `<algorithm>`
    - `<random>`
    - `<unordered_set>`

- `src/Dado.cpp`
  - Importaciones:
    - `"Dado.h"`
    - `<cstdlib>`

- `src/Casilla.cpp`
  - Importaciones:
    - `"Casilla.h"`
    - `"Jugador.h"`
    - `<iostream>`

- `src/Jugador.cpp`
  - Importaciones:
    - `"Jugador.h"`

## assets/

- `assets/Juego.h`
  - Importaciones:
    - `<vector>`
    - `<string>`
    - `<memory>`
    - `"Jugador.h"`
    - `"Casilla.h"`
    - `"Dado.h"`

- `assets/Dado.h`
  - Importaciones: (ninguna adicional)

- `assets/Casilla.h`
  - Importaciones:
    - `<string>`

- `assets/Jugador.h`
  - Importaciones:
    - `<string>`

## qt/

- `qt/main.cpp`
  - Importaciones:
    - `<QApplication>`
    - `"MainWindow.h"`
    - `<cstdlib>`
    - `<ctime>`

- `qt/MainWindow.h`
  - Importaciones:
    - `<QMainWindow>`
    - `<QGraphicsView>`
    - `<QGraphicsScene>`
    - `<QPushButton>`
    - `<QLabel>`
    - `<QListWidget>`
    - `<QDialog>`
    - `<QLineEdit>`
    - `<QSpinBox>`
    - `<memory>`
    - `"../assets/Juego.h"`

- `qt/MainWindow.cpp`
  - Importaciones:
    - `"MainWindow.h"`
    - `<QVBoxLayout>`
    - `<QHBoxLayout>`
    - `<QSplitter>`
    - `<QGraphicsRectItem>`
    - `<QGraphicsEllipseItem>`
    - `<QGraphicsTextItem>`
    - `<QFont>`
    - `<QTimer>`
    - `<QCheckBox>`
    - `<QFileDialog>`
    - `<QFile>`
    - `<QMessageBox>`
    - `<QMap>`
    - `<QTextStream>`
    - `<QDir>`
    - `<QDateTime>`
    - `<algorithm>`
    - `<QSlider>`
    - `<QToolButton>`
    - `<QStyle>`