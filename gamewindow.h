#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
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

    const int boxWidth = 15;
    const int wallWidth = 3;
    int summaryWidth;
    int summaryHeight;
    bool isServer;
    string name;

    Server* server;
    //Client client;
    void setParams(bool isServer, string name, int x, int y, ServerData serverData);
    void keyPressEvent(QKeyEvent *key); //do smth depend on the key pressed
    void initialize(); //Drawing the start field, without any walls
    void update(); //update visual part????
    void move(string direction);
    int movePlayer(int direction); //Move player
    int check(); //Check if smth at this position, like treasure or landmine
    void drawWall(int curXCoor, int curYCoor, int direction);
    void showTreasureText();
    void shoot(string direction);
    void dig();

    void paintEvent(QPaintEvent *e); //show penis
private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
