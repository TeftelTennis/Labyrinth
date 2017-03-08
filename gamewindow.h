#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "hostserver.h"
#include "server.h"
#include "serverdata.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = 0);
    ~GameWindow();

    const int boxWidth = 50;
    const int wallWidth = 10;
    int summaryWidth;
    int summaryHeight;
    int xCoors;
    int yCoors;
    int width;
    int height;
    int playerXCoor;
    int playerYCoor;
    int keys;
    int bullets;
    bool isServer;
    string name;

    Server* server;
    //Client client;
    int getPosFromXCoor();
    int getPosFromYCoor();
    void setParams(bool isServer, string name, int x, int y, ServerData serverData);
    void keyPressEvent(QKeyEvent *key); //do smth depend on the key pressed
    void initialize(); //Drawing the start field, without any walls
    void update(); //update visual part????
    void updateInfo();
    void move(string direction);
    int movePlayer(string direction); //Move player
    int check(); //Check if smth at this position, like treasure or landmine
    void drawWall(int curXCoor, int curYCoor, int direction);
    void showTreasureText();
    void hideTreasureText();
    void shoot(string direction);
    void dig();

private:
    Ui::GameWindow *ui;
    QGraphicsScene *scene;
    QGraphicsRectItem *menu;
    QGraphicsEllipseItem *playerIcon;
    QGraphicsTextItem *controls;
    QGraphicsTextItem *wsad;
    QGraphicsTextItem *sh;
    QGraphicsTextItem *dg;
    QGraphicsTextItem *inventory;
    QGraphicsTextItem *treasureText;
    QGraphicsTextItem *keyNum;
    QGraphicsTextItem *bulletNum;
};

#endif // GAMEWINDOW_H
