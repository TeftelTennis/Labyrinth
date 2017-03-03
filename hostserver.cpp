#include "hostserver.h"
#include "ui_hostserver.h"

HostServer::HostServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HostServer)
{
    ui->setupUi(this);

}

HostServer::~HostServer()
{
    delete ui;
}

void HostServer::on_menuButton_clicked()
{
    hide();
    parentWidget()->show();
}

void HostServer::on_pushButton_clicked()
{
    //TODO: Host server
    GameWindow *game = new GameWindow(this);
    hide();

    game->setWindowTitle("Labyrinth");
    game->setModal(true);
    game->exec();
    show();
}
