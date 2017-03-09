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
#include "gamelog.h"
#include "splitter.h"

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
    bool isMyWindow;
    string name;
    vector<GameLog> gamelogs;
    vector<string> logs;

    int logPosition;

    Server* server;
    //Client client;
    int getPosFromXCoor();
    int getPosFromYCoor();
    int getPosFromXCoors(int x);
    int getPosFromYCoors(int y);

    void drawLines(int width, int height, int sumWidth, int sumHeight);
    void drawField(GameLog *gamelog);
    void drawMyWindow(GameLog *gamelog);
    void drawEnemy(GameLog *gamelog);
    void drawPath(vector<Direction> directions, int x, int y);
    void drawHorizontalWalls(GameLog *gamelog, bool isMine);
    void drawVerticalWalls(GameLog *gamelog, bool isMine);
    void drawWall(int curXCoor, int curYCoor, int direction);
    void drawMenu();

    void setParams(bool isServer, string name, int x, int y, ServerData serverData);
    void initialize(); //Drawing the start field, without any walls
    void update(); //update visual part????
    void doResultOfTurn(string turnn);
    void updateInfo();
    void move(string direction);
    int movePlayer(string direction); //Move player

    void showTreasureText();
    void hideTreasureText();
    void shoot(string direction);
    void dig();

protected slots:
    void keyPressEvent(QKeyEvent *key); //do smth depend on the key pressed

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
