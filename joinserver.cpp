#include "joinserver.h"
#include "ui_joinserver.h"
#include "gamewindow.h"
#include <iostream>

JoinServer::JoinServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinServer)
{
    ui->setupUi(this);
}

JoinServer::~JoinServer()
{
    delete ui;
}

void JoinServer::on_returnButton_clicked()
{
    hide();
    parentWidget()->show();
}

void JoinServer::on_connectButton_clicked()
{
    GameWindow *game = new GameWindow(this);
    game->startJoin(getXCoor(), getYCoor(), getName(), getIp(), getPort());
    std::cerr << "joined";
    hide();
    game->setWindowTitle("Labyrinth");
    game->setModal(true);
    game->exec();
    show();
}

int JoinServer::getXCoor() {
    return ui->xCoors->text().toInt();
}

int JoinServer::getYCoor() {
    return ui->yCoors->text().toInt();
}

string JoinServer::getName() {
    return ui->nameLine->text().toStdString();
}

QString JoinServer::getIp() {
    return ui->ipLine->text();
}

int JoinServer::getPort() {
    return ui->portLine->text().toInt();
}
