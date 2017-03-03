#include "joinserver.h"
#include "ui_joinserver.h"

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
    //TODO: open OpenGL window

}
