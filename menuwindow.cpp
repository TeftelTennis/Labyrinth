#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    join = new JoinServer(this);
    host = new HostServer(this);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_joinButton_clicked()
{
    this->hide();
    join->setModal(true);
    join->show();
}

void MenuWindow::on_hostButton_clicked()
{
    this->hide();
    host->setModal(true);
    host->show();
}
