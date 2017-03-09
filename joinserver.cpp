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
   //QTcpSocket* socket = new QTcpSocket(this);
    GameWindow *game = new GameWindow(this);
    game->startJoin(0, 0, "rofl");
    std::cerr << "joined";
    hide();
    game->setWindowTitle("Labyrinth");
    game->setModal(true);
    game->exec();
    show();
    //TODO: open OpenGL window

}
