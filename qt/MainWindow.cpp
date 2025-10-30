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
#include <QDir>
#include <QDateTime>
#include <algorithm>

static const int ANCHO_VENTANA = 1500;
static const int ALTO_VENTANA = 900;
static const int TAMANO_CASILLA = 88;
static const int MARGEN = 50;
static const int ESPACIADO_CASILLAS = 6;
static const int FILAS_TABLERO = 10;
static const int COLUMNAS_TABLERO = 9;

VentanaPrincipal::VentanaPrincipal(QWidget* parent)
    : QMainWindow(parent),
      vistaTablero(new QGraphicsView(this)),
      escenaTablero(new QGraphicsScene(this)),
      botonLanzar(new QPushButton("LANZAR DADO", this)),
      botonReiniciar(new QPushButton("REINICIAR JUEGO", this)),
      botonGuardarPartida(new QPushButton("GUARDAR PARTIDA...", this)),
      botonCargarPartida(new QPushButton("CARGAR PARTIDA...", this)),
      etiquetaTurno(new QLabel("Turno: Esperando inicio del juego", this)),
      etiquetaDado(new QLabel("Dado: -", this)),
      etiquetaUltimoTiro(new QLabel("Último: -", this)),
      listaHistorial(new QListWidget(this)),
      listaJugadores(new QListWidget(this)) {

    setWindowTitle("Juego de la Oca - Qt");
    resize(ANCHO_VENTANA, ALTO_VENTANA);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* rootLayout = new QHBoxLayout(central);

    // Vista del tablero
    vistaTablero->setScene(escenaTablero);
    vistaTablero->setRenderHint(QPainter::Antialiasing);
    vistaTablero->setMinimumSize(1100, 850);

    // Panel lateral
    QWidget* sidePanel = new QWidget(this);
    QVBoxLayout* sideLayout = new QVBoxLayout(sidePanel);
    QFont f; f.setPointSize(12);
    etiquetaTurno->setFont(f);
    etiquetaDado->setFont(f);
    sidePanel->setMinimumWidth(420);
    sideLayout->addWidget(etiquetaTurno);
    sideLayout->addWidget(etiquetaDado);
    sideLayout->addWidget(botonLanzar);
    sideLayout->addWidget(etiquetaUltimoTiro);
    sideLayout->addWidget(new QLabel("Jugadores:", this));
    sideLayout->addWidget(listaJugadores);
    sideLayout->addWidget(botonReiniciar);
    sideLayout->addWidget(new QLabel("Historial:", this));
    sideLayout->addWidget(listaHistorial, 1);
    sideLayout->addWidget(botonGuardarPartida);
    sideLayout->addWidget(botonCargarPartida);

    rootLayout->addWidget(vistaTablero, 1);
    rootLayout->addWidget(sidePanel);

    connect(botonLanzar, &QPushButton::clicked, this, &VentanaPrincipal::alLanzarDado);
    connect(botonReiniciar, &QPushButton::clicked, this, &VentanaPrincipal::alReiniciar);
    connect(botonGuardarPartida, &QPushButton::clicked, this, &VentanaPrincipal::alGuardarPartida);
    connect(botonCargarPartida, &QPushButton::clicked, this, &VentanaPrincipal::alCargarPartida);

    // Inicializar escena y juego
    construirTablero();
    mostrarDialogoConfiguracion();
}

VentanaPrincipal::~VentanaPrincipal() = default;

void VentanaPrincipal::construirTablero() {
    escenaTablero->clear();
    const int meta = metaTablero_;
    // Dibujar casillas 0..meta
    for (int i = 0; i <= meta; ++i) {
        QPointF p = posicionCasilla(i);
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
        }

        auto* rect = escenaTablero->addRect(r, QPen(Qt::black, 2), QBrush(fill));
        // Número
        auto* tNum = escenaTablero->addText(QString::number(i));
        tNum->setDefaultTextColor(Qt::black);
        tNum->setPos(p.x() + 2, p.y() + 2);
        // Nombre
        if (!name.isEmpty()) {
            auto* tName = escenaTablero->addText(name);
            tName->setDefaultTextColor(Qt::black);
            tName->setPos(p.x() + 2, p.y() + TAMANO_CASILLA - 26);
        }
    }
}

QPointF VentanaPrincipal::posicionCasilla(int indiceCasilla) const {
    int fila = indiceCasilla / COLUMNAS_TABLERO;
    int columna = indiceCasilla % COLUMNAS_TABLERO;
    if (fila % 2 == 1) {
        columna = COLUMNAS_TABLERO - 1 - columna;
    }
    const qreal x = MARGEN + columna * (TAMANO_CASILLA + ESPACIADO_CASILLAS);
    const qreal y = ALTO_VENTANA - MARGEN - (fila + 1) * (TAMANO_CASILLA + ESPACIADO_CASILLAS);
    return QPointF(x, y);
}

void VentanaPrincipal::iniciarJuego(const std::vector<std::string>& nombres, int meta, bool especialesAleatorios, int cantidadDados) {
    metaTablero_ = meta;
    especialesAleatorios_ = especialesAleatorios;
    cantidadDados_ = cantidadDados;
    ultimosNombresJugadores_ = nombres;
    juego = std::make_unique<Juego>(nombres, meta, especialesAleatorios, cantidadDados);
    sincronizarJugadores();
    // Texto de turno inicial
    int actual = juego->obtenerJugadorActual();
    etiquetaTurno->setText(QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre()));
    listaHistorial->clear();
    if (cantidadDados_ == 1) {
        etiquetaDado->setText("Dado: -");
    } else {
        etiquetaDado->setText(QString("Dados (%1): -").arg(cantidadDados_));
    }
    etiquetaUltimoTiro->setText("Último: -");
    botonLanzar->setEnabled(true);
    listaJugadores->clear();
    for (int i = 0; i < juego->obtenerCantidadJugadores(); ++i) {
        const auto& j = juego->obtenerJugador(i);
        QString s = QString::fromStdString(j.conseguirNombre()) +
                    QString(" (Casilla %1)").arg(j.conseguirPosicion());
        if (j.estaEnPozo()) s += " - EN POZO";
        if (j.getTurnosPerdidos() > 0) s += QString(" - Pierde %1 turnos").arg(j.getTurnosPerdidos());
        listaJugadores->addItem(s);
    }
    construirTablero();
}

void VentanaPrincipal::sincronizarJugadores() {
    // Eliminar fichas anteriores
    QList<QGraphicsItem*> toRemove;
    for (auto* item : escenaTablero->items()) {
        if (item->data(0).toString() == "ficha") {
            toRemove.push_back(item);
        }
    }
    for (auto* item : toRemove) escenaTablero->removeItem(item), delete item;

    // Dibujar fichas actuales
    const int n = juego->obtenerCantidadJugadores();
    const QColor colors[4] = {Qt::red, Qt::blue, Qt::green, Qt::yellow};
    for (int i = 0; i < n; ++i) {
        int pos = juego->obtenerJugador(i).conseguirPosicion();
        QPointF p = posicionCasilla(pos);
        // separacion para multiples jugadores
        p.rx() += (i % 2) * 10 + 5;
        p.ry() += (i / 2) * 10 + 5;
        auto* circ = escenaTablero->addEllipse(QRectF(p.x(), p.y(), 18, 18), QPen(Qt::black), QBrush(colors[i % 4]));
        circ->setData(0, "ficha");
    }

    // Actualizar lista de jugadores y estados
    listaJugadores->clear();
    for (int i = 0; i < n; ++i) {
        const auto& j = juego->obtenerJugador(i);
        QString s = QString::fromStdString(j.conseguirNombre()) +
                    QString(" (Casilla %1)").arg(j.conseguirPosicion());
        if (j.estaEnPozo()) s += " - EN POZO";
        if (j.getTurnosPerdidos() > 0) s += QString(" - Pierde %1 turnos").arg(j.getTurnosPerdidos());
        listaJugadores->addItem(s);
    }
}

void VentanaPrincipal::alLanzarDado() {
    if (!juego || !juego->estaJugando()) return;
    const ResultadoTurno res = juego->lanzarDadoYJugarTurno();
    if (juego->obtenerCantidadDados() == 1) {
        etiquetaDado->setText(QString::fromStdString("Dado: " + std::to_string(res.resultadoDado)));
    } else {
        etiquetaDado->setText(QString::fromStdString("Dados (" + std::to_string(juego->obtenerCantidadDados()) + "): " + std::to_string(res.resultadoDado)));
    }
    // Último tiro
    int previo = (juego->obtenerJugadorActual() - 1);
    if (previo < 0) previo = juego->obtenerCantidadJugadores() - 1;
    QString nombrePrevio = QString::fromStdString(juego->obtenerJugador(previo).conseguirNombre());
    etiquetaUltimoTiro->setText(QString("Último: %1 → %2").arg(nombrePrevio).arg(res.resultadoDado));
    // Agregar al historial (dividir en líneas si hay saltos)
    const QString desc = QString::fromStdString(res.descripcion);
    for (const QString& line : desc.split('\n')) {
        if (!line.trimmed().isEmpty()) listaHistorial->addItem(line);
    }
    // Actualizar turno
    int actual = juego->obtenerJugadorActual();
    QString turno = QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre());
    if (juego->obtenerJugador(actual).estaEnPozo()) turno += " (EN POZO)";
    if (juego->obtenerJugador(actual).getTurnosPerdidos() > 0) turno += QString(" (Pierde %1 turnos)").arg(juego->obtenerJugador(actual).getTurnosPerdidos());
    etiquetaTurno->setText(turno);

    sincronizarJugadores();

    // Si el juego ha terminado, deshabilitar el botón y mostrar ganador
    if (!juego->estaJugando()) {
        botonLanzar->setEnabled(false);
        QString ganador = QString::fromStdString(juego->obtenerJugador(juego->obtenerJugadorActual()).conseguirNombre());
        etiquetaTurno->setText(QString("Juego finalizado: %1 ha ganado").arg(ganador));
    }
}

void VentanaPrincipal::alReiniciar() {
    if (!juego) return;
    juego->reiniciarJuego();
    cantidadDados_ = juego->obtenerCantidadDados();
    listaHistorial->clear();
    if (juego->obtenerCantidadDados() == 1) {
        etiquetaDado->setText("Dado: -");
    } else {
        etiquetaDado->setText(QString("Dados (%1): -").arg(juego->obtenerCantidadDados()));
    }
    etiquetaUltimoTiro->setText("Último: -");
    botonLanzar->setEnabled(true);
    int actual = juego->obtenerJugadorActual();
    etiquetaTurno->setText(QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre()));
    sincronizarJugadores();
    construirTablero();
}

void VentanaPrincipal::alGuardarPartida() {
    if (!juego) {
        QMessageBox::information(this, "Guardar partida", "No hay partida en curso.");
        return;
    }
    // Asegurar carpeta saves/
    QDir dir(QDir::current());
    if (!dir.exists("saves")) {
        dir.mkpath("saves");
    }
    const QString sugerido = QString("Partida-%1.oca").arg(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"));
    const QString rutaInicial = dir.filePath(QString("saves/%1").arg(sugerido));
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar partida", rutaInicial, "Partidas (*.oca);;Todos los archivos (*.*)");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".oca")) fileName += ".oca";

    std::vector<std::string> historial;
    historial.reserve(listaHistorial->count());
    for (int i = 0; i < listaHistorial->count(); ++i) {
        historial.push_back(listaHistorial->item(i)->text().toStdString());
    }

    bool ok = juego->guardarPartida(fileName.toStdString(), historial);
    if (!ok) {
        QMessageBox::warning(this, "Error", "No se pudo guardar la partida.");
    } else {
        QMessageBox::information(this, "Guardar partida", "Partida guardada correctamente.");
    }
}

void VentanaPrincipal::alCargarPartida() {
    // Abrir desde carpeta saves/ por defecto
    QDir dir(QDir::current());
    if (!dir.exists("saves")) {
        dir.mkpath("saves");
    }
    const QString rutaInicial = dir.filePath("saves/");
    QString fileName = QFileDialog::getOpenFileName(this, "Cargar partida", rutaInicial, "Partidas (*.oca);;Todos los archivos (*.*)");
    if (fileName.isEmpty()) return;

    std::vector<std::string> historial;
    std::unique_ptr<Juego> cargado = Juego::cargarPartida(fileName.toStdString(), historial);
    if (!cargado) {
        QMessageBox::warning(this, "Error", "No se pudo cargar la partida.");
        return;
    }

    // Adoptar juego cargado
    juego = std::move(cargado);
    metaTablero_ = juego->obtenerMeta();
    especialesAleatorios_ = juego->obtenerEspecialesAleatorios();
    cantidadDados_ = juego->obtenerCantidadDados();

    // Actualizar UI básica
    listaHistorial->clear();
    for (const auto& linea : historial) listaHistorial->addItem(QString::fromStdString(linea));

    etiquetaUltimoTiro->setText("Último: -");
    if (cantidadDados_ == 1) {
        etiquetaDado->setText("Dado: -");
    } else {
        etiquetaDado->setText(QString("Dados (%1): -").arg(cantidadDados_));
    }

    // Nombres recordados
    ultimosNombresJugadores_.clear();
    for (int i = 0; i < juego->obtenerCantidadJugadores(); ++i) {
        ultimosNombresJugadores_.push_back(juego->obtenerJugador(i).conseguirNombre());
    }

    // Turno y jugadores
    int actual = juego->obtenerJugadorActual();
    QString turno = QString::fromStdString("Turno: " + juego->obtenerJugador(actual).conseguirNombre());
    if (juego->obtenerJugador(actual).estaEnPozo()) turno += " (EN POZO)";
    if (juego->obtenerJugador(actual).getTurnosPerdidos() > 0) turno += QString(" (Pierde %1 turnos)").arg(juego->obtenerJugador(actual).getTurnosPerdidos());
    etiquetaTurno->setText(turno);

    // Redibujar
    construirTablero();
    sincronizarJugadores();

    // Habilitar/deshabilitar
    botonLanzar->setEnabled(juego->estaJugando());
}

void VentanaPrincipal::mostrarDialogoConfiguracion() {
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
    spinMeta->setValue(metaTablero_);
    v->addWidget(lmeta);
    v->addWidget(spinMeta);

    // Especiales aleatorios
    QCheckBox* chkAleatorio = new QCheckBox("Generar casillas especiales al azar", &dlg);
    chkAleatorio->setChecked(especialesAleatorios_);
    v->addWidget(chkAleatorio);

    // Cantidad de dados
    QCheckBox* chkDosDados = new QCheckBox("Jugar con dos dados", &dlg);
    chkDosDados->setChecked(cantidadDados_ == 2);
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

    int initialPlayers = static_cast<int>(std::clamp<std::size_t>(ultimosNombresJugadores_.size(), 2u, 4u));
    spin->setValue(initialPlayers);
    for (int i = 0; i < 4; ++i) {
        if (i < initialPlayers && i < static_cast<int>(ultimosNombresJugadores_.size())) {
            edits[i]->setText(QString::fromStdString(ultimosNombresJugadores_[i]));
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
        int metaVal = values.value("meta", QString::number(metaTablero_)).toInt(&ok);
        if (ok) {
            metaVal = std::clamp(metaVal, 63, 90);
            spinMeta->setValue(metaVal);
        }

        int randVal = values.value("randomSpecials", QString::number(especialesAleatorios_ ? 1 : 0)).toInt(&ok);
        if (ok) chkAleatorio->setChecked(randVal != 0);

        int diceVal = values.value("diceCount", QString::number(cantidadDados_)).toInt(&ok);
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
        iniciarJuego(nombres, meta, ale, dados);
    } else if (!juego) {
        // fallback inicial si todavía no se ha creado un juego
        iniciarJuego(ultimosNombresJugadores_, metaTablero_, especialesAleatorios_, cantidadDados_);
    }
}


