#ifndef HOSTSERVER_H
#define HOSTSERVER_H

#include <QDialog>
#include "gamewindow.h"

namespace Ui {
class HostServer;
}

class HostServer : public QDialog
{
    Q_OBJECT

public:
    explicit HostServer(QWidget *parent = 0);
    ~HostServer();

    int getXCoor();
    int getYCoor();
    int getWidth();
    int getHeight();
    int getStartAmmo();
    int getStartLifes();
    int getKeys();
    int getBullets();
    int getMines();

    float getWallProb();
    float getStaticTreasureProb();
    float getLoveToiletsProb();

    bool canPutTreasureTogether();
    bool useRandomTreasure();



private slots:
    void on_menuButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::HostServer *ui;
};

#endif // HOSTSERVER_H
