#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QTimer>
#include <QCheckBox>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QMap>
#include <QTextStream>
#include <algorithm>

static const int ANCHO_VENTANA = 1500;
static const int ALTO_VENTANA = 900;
static const int TAMANO_CASILLA = 78;
static const int MARGEN = 50;
static const int ESPACIADO_CASILLAS = 6;
static const int FILAS_TABLERO = 10;
static const int COLUMNAS_TABLERO = 9;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      boardView(new QGraphicsView(this)),
      boardScene(new QGraphicsScene(this)),
      rollButton(new QPushButton("LANZAR DADO", this)),
      restartButton(new QPushButton("REINICIAR JUEGO", this)),
      turnLabel(new QLabel("Turno: Esperando inicio del juego", this)),
      diceLabel(new QLabel("Dado: -", this)),
      lastRollLabel(new QLabel("Último: -", this)),
      historyList(new QListWidget(this)),
      playersList(new QListWidget(this)) {

    setWindowTitle("Juego de la Oca - Qt");
    resize(ANCHO_VENTANA, ALTO_VENTANA);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* rootLayout = new QHBoxLayout(central);

    // Vista del tablero
    boardView->setScene(boardScene);
    boardView->setRenderHint(QPainter::Antialiasing);
    boardView->setMinimumSize(1100, 850);

    // Panel lateral
    QWidget* sidePanel = new QWidget(this);
    QVBoxLayout* sideLayout = new QVBoxLayout(sidePanel);
    QFont f; f.setPointSize(12);
    turnLabel->setFont(f);
    diceLabel->setFont(f);
    sideLayout->addWidget(turnLabel);
    sideLayout->addWidget(diceLabel);
    sideLayout->addWidget(rollButton);
    sideLayout->addWidget(restartButton);
    sideLayout->addWidget(lastRollLabel);
    sideLayout->addWidget(new QLabel("Jugadores:", this));
    sideLayout->addWidget(playersList);
    sideLayout->addWidget(new QLabel("Historial:", this));
    sideLayout->addWidget(historyList, 1);

    rootLayout->addWidget(boardView, 1);
    rootLayout->addWidget(sidePanel);

    connect(rollButton, &QPushButton::clicked, this, &MainWindow::onRollDice);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::onRestart);

    // Inicializar escena y juego
    buildBoard();
    showConfigDialog();
}

MainWindow::~MainWindow() = default;

void MainWindow::buildBoard() {
    boardScene->clear();
    const int meta = boardMeta_;
    // Dibujar casillas 0..meta
    for (int i = 0; i <= meta; ++i) {
        QPointF p = cellPos(i);
        QRectF r(p.x(), p.y(), TAMANO_CASILLA, TAMANO_CASILLA);

        QColor fill(255, 255, 255);
        // Colores: consultamos el nombre real si existe juego, si no usamos heurística
        QString name;
        if (juego) {
            std::string n = (i == 0) ? std::string("") : juego->obtenerNombreCasilla(i);
            if (n == "Puente") { fill = QColor(255, 120, 0); name = "PUENTE"; }
            else if (n == "Posada") { fill = QColor(255, 120, 0); name = "POSADA"; }
            else if (n == "Pozo") { fill = QColor(255, 38, 38); name = "POZO"; }
            else if (n == "Laberinto") { fill = QColor(69, 128, 252); name = "LABERINTO"; }
            else if (n == "Cárcel") { fill = QColor(69, 69, 69); name = "CARCEL"; }
            else if (n == "Calavera") { fill = QColor(153, 122, 111); name = "CALAVERA"; }
            else if (i == meta) { fill = QColor(49, 255, 0); name = "META"; }
            else if (n == "Oca") { fill = QColor(255, 215, 0); name = "OCA"; }
        } else {
            if (i == 6) fill = QColor(255, 120, 0);
            else if (i == 19) fill = QColor(255, 120, 0);
            else if (i == 31) fill = QColor(255, 38, 38);
            else if (i == 42) fill = QColor(69, 128, 252);
            else if (i == 56) fill = QColor(69, 69, 69);
            else if (i == 58) fill = QColor(153, 122, 111);
            else if (i == meta) fill = QColor(49, 255, 0);
            else if (i % 9 == 0 && i < meta && i > 0) fill = QColor(255, 215, 0);
        }

        auto* rect = boardScene->addRect(r, QPen(Qt::black, 2), QBrush(fill));
        // Número
        auto* tNum = boardScene->addText(QString::number(i));
        tNum->setDefaultTextColor(Qt::black);
        tNum->setPos(p.x() + 2, p.y() + 2);
        // Nombre
        if (!name.isEmpty()) {
            auto* tName = boardScene->addText(name);
            tName->setDefaultTextColor(Qt::black);
            tName->setPos(p.x() + 2, p.y() + TAMANO_CASILLA - 18);
        }
    }
}

QPointF MainWindow::cellPos(int numeroCasilla) const {
    int fila = numeroCasilla / COLUMNAS_TABLERO;
    int columna = numeroCasilla % COLUMNAS_TABLERO;
    if (fila % 2 == 1) {
        columna = COLUMNAS_TABLERO - 1 - columna;
    }
    const qreal x = MARGEN + columna * (TAMANO_CASILLA + ESPACIADO_CASILLAS);
    const qreal y = ALTO_VENTANA - MARGEN - (fila + 1) * (TAMANO_CASILLA + ESPACIADO_CASILLAS);
    return QPointF(x, y);
}

void MainWindow::initGame(const std::vector<std::string>& nombres, int meta, bool especialesAleatorios, int cantidadDados) {
    boardMeta_ = meta;
    randomSpecials_ = especialesAleatorios;
    diceCount_ = cantidadDados;
    lastPlayerNames_ = nombres;
    juego = std::make_unique<Juego>(nombres, meta, especialesAleatorios, cantidadDados);
    syncPlayers();
    // Texto de turno inicial
    int actual = juego->obtenerJugadorActual();
    turnLabel->setText(QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre()));
    historyList->clear();
    if (diceCount_ == 1) {
        diceLabel->setText("Dado: -");
    } else {
        diceLabel->setText(QString("Dados (%1): -").arg(diceCount_));
    }
    lastRollLabel->setText("Último: -");
    rollButton->setEnabled(true);
    playersList->clear();
    for (int i = 0; i < juego->obtenerCantidadJugadores(); ++i) {
        const auto& j = juego->obtenerJugador(i);
        QString s = QString::fromStdString(j.conseguirNombre()) +
                    QString(" (Casilla %1)").arg(j.conseguirPosicion());
        if (j.estaEnPozo()) s += " - EN POZO";
        if (j.getTurnosPerdidos() > 0) s += QString(" - Pierde %1 turnos").arg(j.getTurnosPerdidos());
        playersList->addItem(s);
    }
    buildBoard();
}

void MainWindow::syncPlayers() {
    // Eliminar fichas anteriores
    QList<QGraphicsItem*> toRemove;
    for (auto* item : boardScene->items()) {
        if (item->data(0).toString() == "ficha") {
            toRemove.push_back(item);
        }
    }
    for (auto* item : toRemove) boardScene->removeItem(item), delete item;

    // Dibujar fichas actuales
    const int n = juego->obtenerCantidadJugadores();
    const QColor colors[4] = {Qt::red, Qt::blue, Qt::green, Qt::yellow};
    for (int i = 0; i < n; ++i) {
        int pos = juego->obtenerJugador(i).conseguirPosicion();
        QPointF p = cellPos(pos);
        // separacion para multiples jugadores
        p.rx() += (i % 2) * 10 + 5;
        p.ry() += (i / 2) * 10 + 5;
        auto* circ = boardScene->addEllipse(QRectF(p.x(), p.y(), 18, 18), QPen(Qt::black), QBrush(colors[i % 4]));
        circ->setData(0, "ficha");
    }

    // Actualizar lista de jugadores y estados
    playersList->clear();
    for (int i = 0; i < n; ++i) {
        const auto& j = juego->obtenerJugador(i);
        QString s = QString::fromStdString(j.conseguirNombre()) +
                    QString(" (Casilla %1)").arg(j.conseguirPosicion());
        if (j.estaEnPozo()) s += " - EN POZO";
        if (j.getTurnosPerdidos() > 0) s += QString(" - Pierde %1 turnos").arg(j.getTurnosPerdidos());
        playersList->addItem(s);
    }
}

void MainWindow::onRollDice() {
    if (!juego || !juego->estaJugando()) return;
    const ResultadoTurno res = juego->lanzarDadoYJugarTurno();
    if (juego->obtenerCantidadDados() == 1) {
        diceLabel->setText(QString::fromStdString("Dado: " + std::to_string(res.resultadoDado)));
    } else {
        diceLabel->setText(QString::fromStdString("Dados (" + std::to_string(juego->obtenerCantidadDados()) + "): " + std::to_string(res.resultadoDado)));
    }
    // Último tiro
    int previo = (juego->obtenerJugadorActual() - 1);
    if (previo < 0) previo = juego->obtenerCantidadJugadores() - 1;
    QString nombrePrevio = QString::fromStdString(juego->obtenerJugador(previo).conseguirNombre());
    lastRollLabel->setText(QString("Último: %1 → %2").arg(nombrePrevio).arg(res.resultadoDado));
    // Agregar al historial (dividir en líneas si hay saltos)
    const QString desc = QString::fromStdString(res.descripcion);
    for (const QString& line : desc.split('\n')) {
        if (!line.trimmed().isEmpty()) historyList->addItem(line);
    }
    // Actualizar turno
    int actual = juego->obtenerJugadorActual();
    QString turno = QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre());
    if (juego->obtenerJugador(actual).estaEnPozo()) turno += " (EN POZO)";
    if (juego->obtenerJugador(actual).getTurnosPerdidos() > 0) turno += QString(" (Pierde %1 turnos)").arg(juego->obtenerJugador(actual).getTurnosPerdidos());
    turnLabel->setText(turno);

    syncPlayers();

    // Si el juego ha terminado, deshabilitar el botón y mostrar ganador
    if (!juego->estaJugando()) {
        rollButton->setEnabled(false);
        QString ganador = QString::fromStdString(juego->obtenerJugador(juego->obtenerJugadorActual()).conseguirNombre());
        turnLabel->setText(QString("Juego finalizado: %1 ha ganado").arg(ganador));
    }
}

void MainWindow::onRestart() {
    if (!juego) return;
    juego->reiniciarJuego();
    diceCount_ = juego->obtenerCantidadDados();
    historyList->clear();
    if (juego->obtenerCantidadDados() == 1) {
        diceLabel->setText("Dado: -");
    } else {
        diceLabel->setText(QString("Dados (%1): -").arg(juego->obtenerCantidadDados()));
    }
    lastRollLabel->setText("Último: -");
    rollButton->setEnabled(true);
    int actual = juego->obtenerJugadorActual();
    turnLabel->setText(QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre()));
    syncPlayers();
    buildBoard();
}

void MainWindow::showConfigDialog() {
    QDialog dlg(this);
    dlg.setWindowTitle("Configuración del juego");
    QVBoxLayout* v = new QVBoxLayout(&dlg);
    QLabel* lnum = new QLabel("Cantidad de jugadores (2-4):", &dlg);
    QSpinBox* spin = new QSpinBox(&dlg);
    spin->setRange(2, 4);
    v->addWidget(lnum);
    v->addWidget(spin);

    // Meta
    QLabel* lmeta = new QLabel("Casillas del tablero (63-90):", &dlg);
    QSpinBox* spinMeta = new QSpinBox(&dlg);
    spinMeta->setRange(63, 90);
    spinMeta->setValue(boardMeta_);
    v->addWidget(lmeta);
    v->addWidget(spinMeta);

    // Especiales aleatorios
    QCheckBox* chkAleatorio = new QCheckBox("Generar casillas especiales al azar", &dlg);
    chkAleatorio->setChecked(randomSpecials_);
    v->addWidget(chkAleatorio);

    // Cantidad de dados
    QCheckBox* chkDosDados = new QCheckBox("Jugar con dos dados", &dlg);
    chkDosDados->setChecked(diceCount_ == 2);
    v->addWidget(chkDosDados);

    // Contenedores de nombres
    std::vector<QLineEdit*> edits;
    for (int i = 0; i < 4; ++i) {
        QLineEdit* e = new QLineEdit(&dlg);
        e->setPlaceholderText(QString("Jugador %1").arg(i + 1));
        edits.push_back(e);
        v->addWidget(e);
    }

    QObject::connect(spin, qOverload<int>(&QSpinBox::valueChanged), &dlg, [&](int n){
        for (int i = 0; i < 4; ++i) {
            edits[i]->setVisible(i < n);
            if (i < n && edits[i]->text().trimmed().isEmpty()) {
                edits[i]->setText(QString("Jugador %1").arg(i + 1));
            }
        }
    });

    int initialPlayers = static_cast<int>(std::clamp<std::size_t>(lastPlayerNames_.size(), 2u, 4u));
    spin->setValue(initialPlayers);
    for (int i = 0; i < 4; ++i) {
        if (i < initialPlayers && i < static_cast<int>(lastPlayerNames_.size())) {
            edits[i]->setText(QString::fromStdString(lastPlayerNames_[i]));
        } else if (i < initialPlayers) {
            edits[i]->setText(QString("Jugador %1").arg(i + 1));
        }
        edits[i]->setVisible(i < initialPlayers);
    }

    // Botones para guardar/cargar configuración
    QHBoxLayout* saveLoadLayout = new QHBoxLayout();
    QPushButton* btnSave = new QPushButton("Guardar configuración...", &dlg);
    QPushButton* btnLoad = new QPushButton("Cargar configuración...", &dlg);
    saveLoadLayout->addWidget(btnSave);
    saveLoadLayout->addWidget(btnLoad);
    v->addLayout(saveLoadLayout);

    QObject::connect(btnSave, &QPushButton::clicked, &dlg, [&](){
        QString fileName = QFileDialog::getSaveFileName(&dlg, "Guardar configuración", QString(), "Archivos de texto (*.txt);;Todos los archivos (*.*)");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(&dlg, "Error", "No se pudo guardar la configuración.");
            return;
        }

        QTextStream out(&file);
        out << "meta=" << spinMeta->value() << '\n';
        out << "randomSpecials=" << (chkAleatorio->isChecked() ? 1 : 0) << '\n';
        out << "diceCount=" << (chkDosDados->isChecked() ? 2 : 1) << '\n';
        QStringList nombres;
        for (int i = 0; i < spin->value(); ++i) {
            QString nombre = edits[i]->text().trimmed();
            if (nombre.isEmpty()) nombre = QString("Jugador %1").arg(i + 1);
            nombres << nombre;
        }
        out << "players=" << nombres.join(';') << '\n';
        file.close();
    });

    QObject::connect(btnLoad, &QPushButton::clicked, &dlg, [&](){
        QString fileName = QFileDialog::getOpenFileName(&dlg, "Cargar configuración", QString(), "Archivos de texto (*.txt);;Todos los archivos (*.*)");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(&dlg, "Error", "No se pudo abrir el archivo de configuración.");
            return;
        }

        QTextStream in(&file);
        QMap<QString, QString> values;
        while (!in.atEnd()) {
            const QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#')) continue;
            const int idx = line.indexOf('=');
            if (idx < 0) continue;
            QString key = line.left(idx).trimmed();
            QString value = line.mid(idx + 1).trimmed();
            values.insert(key, value);
        }
        file.close();

        bool ok = false;
        int metaVal = values.value("meta", QString::number(boardMeta_)).toInt(&ok);
        if (ok) {
            metaVal = std::clamp(metaVal, 63, 90);
            spinMeta->setValue(metaVal);
        }

        int randVal = values.value("randomSpecials", QString::number(randomSpecials_ ? 1 : 0)).toInt(&ok);
        if (ok) chkAleatorio->setChecked(randVal != 0);

        int diceVal = values.value("diceCount", QString::number(diceCount_)).toInt(&ok);
        if (ok) chkDosDados->setChecked(diceVal >= 2);

        QString playersLine = values.value("players");
        QStringList rawNames = playersLine.split(';', Qt::SkipEmptyParts);
        if (rawNames.size() < 2) {
            rawNames.clear();
            rawNames << "Jugador 1" << "Jugador 2";
        }
        if (rawNames.size() > 4) rawNames = rawNames.mid(0, 4);

        int newCount = rawNames.size();
        spin->setValue(newCount);
        for (int i = 0; i < 4; ++i) {
            if (i < rawNames.size()) {
                edits[i]->setText(rawNames[i].trimmed());
            } else if (i < newCount) {
                edits[i]->setText(QString("Jugador %1").arg(i + 1));
            } else {
                edits[i]->clear();
            }
            edits[i]->setVisible(i < newCount);
        }
    });

    QHBoxLayout* h = new QHBoxLayout();
    QPushButton* ok = new QPushButton("Iniciar", &dlg);
    QPushButton* cancel = new QPushButton("Cancelar", &dlg);
    h->addWidget(ok);
    h->addWidget(cancel);
    v->addLayout(h);
    QObject::connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    QObject::connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        int cant = spin->value();
        std::vector<std::string> nombres;
        for (int i = 0; i < cant; ++i) {
            QString t = edits[i]->text().trimmed();
            if (t.isEmpty()) t = QString("Jugador %1").arg(i + 1);
            nombres.push_back(t.toStdString());
        }
        int meta = spinMeta->value();
        bool ale = chkAleatorio->isChecked();
        int dados = chkDosDados->isChecked() ? 2 : 1;
        initGame(nombres, meta, ale, dados);
    } else if (!juego) {
        // fallback inicial si todavía no se ha creado un juego
        initGame(lastPlayerNames_, boardMeta_, randomSpecials_, diceCount_);
    }
}


