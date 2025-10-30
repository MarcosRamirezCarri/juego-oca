#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <memory>

#include "../assets/Juego.h"

class VentanaPrincipal : public QMainWindow {
    Q_OBJECT
public:
    explicit VentanaPrincipal(QWidget* parent = nullptr);
    ~VentanaPrincipal() override;

private slots:
    void alLanzarDado();
    void alReiniciar();
    void alGuardarPartida();
    void alCargarPartida();

private:
    // UI básica
    QGraphicsView* vistaTablero;
    QGraphicsScene* escenaTablero;
    QPushButton* botonLanzar;
    QPushButton* botonReiniciar;
    QPushButton* botonGuardarPartida;
    QPushButton* botonCargarPartida;
    QLabel* etiquetaTurno;
    QLabel* etiquetaDado;
    QLabel* etiquetaUltimoTiro; // último tiro + jugador
    QListWidget* listaHistorial;
    QListWidget* listaJugadores; // lista de jugadores y estados

    // Juego
    std::unique_ptr<Juego> juego;

    // Métodos auxiliares
    void construirTablero();
    QPointF posicionCasilla(int indiceCasilla) const; // 0..meta
    void sincronizarJugadores();
    void iniciarJuego(const std::vector<std::string>& nombres, int meta, bool especialesAleatorios, int cantidadDados);
    void mostrarDialogoConfiguracion();

    // Config actual
    int metaTablero_ = 63;
    bool especialesAleatorios_ = false;
    int cantidadDados_ = 1;
    std::vector<std::string> ultimosNombresJugadores_ = {"Jugador 1", "Jugador 2"};
};

#endif // VENTANAPRINCIPAL_H


