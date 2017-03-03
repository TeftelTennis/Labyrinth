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
    game->setParams(getWidth(), getHeight(), getXCoor(), getYCoor(), getStartAmmo(),
                    getStartLifes(), getKeys(), getBullets(), getMines());
    game->setParamsFloat(getWallProb(), getStaticTreasureProb(), getLoveToiletsProb(),
                         canPutTreasureTogether(), useRandomTreasure());
    game->setWindowTitle("Labyrinth");
    game->setModal(true);
    game->exec();
    show();
}

int HostServer::getXCoor() {
    return ui->xCoors->text().toInt();
}

int HostServer::getYCoor() {
    return ui->yCoors->text().toInt();
}

int HostServer::getWidth() {
    return ui->widthSpinbox->value();
}

int HostServer::getHeight() {
    return ui->heightSpinbox->value();
}

int HostServer::getStartAmmo() {
    return ui->ammoSpinbox->value();
}

int HostServer::getStartLifes() {
    return ui->lifeSpinbox->value();
}

int HostServer::getKeys() {
    return ui->keysSpinbox->value();
}

int HostServer::getBullets() {
    return ui->bulletsSpinbox->value();
}

int HostServer::getMines() {
    return ui->minesSpinbox->value();
}

float HostServer::getWallProb() {
    return (ui->wallProb->value()) / 100;
}

float HostServer::getStaticTreasureProb() {
    return (ui->tresureProb->value()) / 100;
}

float HostServer::getLoveToiletsProb() {
    return (ui->toiletProb->value()) / 100;
}

bool HostServer::canPutTreasureTogether() {
    return ui->treasureTogether->isChecked();
}

bool HostServer::useRandomTreasure() {
    return ui->randomTreasure->isChecked();
}
