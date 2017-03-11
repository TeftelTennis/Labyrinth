#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>

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
    int keys;
    int bullets;
    int life;
    bool isServer;
    bool isMyWindow;
    bool isInitialised;
    bool isTreasure;
    string name;
    string winner;
    string nameNext;
    vector<GameLog> gamelogs;
    vector<string> logs;

    int logPosition;

    int server_status;
    QMap<int,QTcpSocket *> SClients;
    QTcpServer *tcpServer;

    QTcpSocket* tcpSocket;
    QDataStream in;


    Server* server;

    int getPosFromXCoors(int x);
    int getPosFromYCoors(int y);

    void addLog(string s);

    void drawLine(int xFrom, int yFrom, string direction);
    void drawLines(int width, int height, int sumWidth, int sumHeight);
    void drawField(GameLog *gamelog);
    void drawMyWindow(GameLog *gamelog);
    void drawEnemy(GameLog *gamelog);
    void drawPath(vector<Direction> directions, int x, int y);
    void drawHorizontalWalls(GameLog *gamelog);
    void drawVerticalWalls(GameLog *gamelog);
    void drawWall(int curXCoor, int curYCoor, int direction);
    void drawMenu();

    void setServerParams(string name, int x, int y, ServerData serverData);
    void initialize(); //Drawing the start field, without any walls
    void update(); //update visual part
    void doResultOfTurn(string turnn);
    void move(string direction);

    void showTreasureText();
    void hideTreasureText();
    void shoot(string direction);
    void dig();
    void sendtoall(string msg);
    void sendtoserver(string msg);
    void startJoin(int x, int y, string name);

protected slots:
    void keyPressEvent(QKeyEvent *key); //do smth depend on the key pressed

private slots:
    void newuser();

    void slotReadClient();

    void readDataFromServer();

private:
    Ui::GameWindow *ui;

    QWidget *widg;
    QScrollArea *area;
    QVBoxLayout *layout;

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
