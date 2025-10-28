#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onRollDice();
    void onRestart();

private:
    // UI básica
    QGraphicsView* boardView;
    QGraphicsScene* boardScene;
    QPushButton* rollButton;
    QPushButton* restartButton;
    QLabel* turnLabel;
    QLabel* diceLabel;
    QLabel* lastRollLabel; // último tiro + jugador
    QListWidget* historyList;
    QListWidget* playersList; // lista de jugadores y estados

    // Juego
    std::unique_ptr<Juego> juego;

    // Métodos auxiliares
    void buildBoard();
    QPointF cellPos(int cellIndex) const; // 0..meta
    void syncPlayers();
    void initGame(const std::vector<std::string>& nombres, int meta, bool especialesAleatorios);
    void showConfigDialog();

    // Config actual
    int boardMeta_ = 63;
    bool randomSpecials_ = false;
};

#endif // MAINWINDOW_H


