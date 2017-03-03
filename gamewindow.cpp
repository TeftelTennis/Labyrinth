#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::keyPressEvent(QKeyEvent *key) {
    switch (key->key()) {
        case Qt::Key_Escape:
            close();
    }
}

void GameWindow::initialize() {

}

void GameWindow::update() {

}

void GameWindow::movePlayer(double xFrom, double yFrom, double xTo, double yTo) {

}

void GameWindow::check() {

}

void GameWindow::paintEvent(QPaintEvent *e) {
    QPainter painter(this);

    painter.drawLine(50, 100, 100, 100);
    painter.drawLine(75, 100, 75, 75);
    painter.drawLine(50, 100, 50, 75);
    painter.drawLine(100, 100, 100, 75);
    painter.drawLine(50, 75, 100, 75);
    painter.drawLine(65, 75, 65, 10);
    painter.drawLine(85, 75, 85, 10);
    painter.drawLine(65, 10, 85, 10);
    painter.drawLine(65, 20, 85, 20);
}
