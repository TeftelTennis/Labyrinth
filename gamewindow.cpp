#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <iostream>

using namespace std;

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    cerr << "created";

}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::setParams(bool isServer, string name, int x, int y, ServerData serverData) {

      cerr << "paramsstart";
    this->name = name;

    this->isServer = isServer;
    if (isServer) {
        server = new Server(serverData);
        server->addPlayer(x, y, name);
        server->addPlayer(0, 1, "nigga");
    }
    else {
        //client = Client(serverData);
        //clientAddPlayer(name, x, y);
    }
    summaryWidth = serverData.width * boxWidth + (serverData.width - 1) * wallWidth;
    summaryHeight = serverData.height * boxWidth + (serverData.height - 1) * wallWidth;
}


void GameWindow::keyPressEvent(QKeyEvent *key) {
    switch (key->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Up:
            move("up");
            break;
        case Qt::Key_Right:
            move("right");
            break;
        case Qt::Key_Left:
            move("left");
            break;
        case Qt::Key_Down:
            move("down");
            break;
        case Qt::Key_W:
            shoot("up");
            break;
        case Qt::Key_A:
            shoot("left");
            break;
        case Qt::Key_S:
            shoot("down");
            break;
        case Qt::Key_D:
            shoot("right");
            break;
        case Qt::Key_Q:
            dig();
            break;
    }
}

void GameWindow::initialize() {
    //creates a new field for current player
    //menu AlignRight with keys description
}

void GameWindow::update() {

}

void GameWindow::move(string direction) {

    if (isServer) {
        server->move(name, direction);
    }
    else {
        //
    }


    /*
    int i = movePlayer(direction);
    switch (i) {
        case 0:
            //moving the player depends on direction
            update();
            break;
        case 1:
            drawWall(xCoors, yCoors, direction);
            break;
        case 2:
            //some func moving player
            showTreasureText();
            break;
        case 3:
            //player life's -1 and checkIfDead()
            break;
    }*/
}

int GameWindow::movePlayer(int direction) { //direction: 0 - left, 1 - up, 2 - right, 3 - down
    //return 0 if player can get to that direction
    //return 1 if there is a wall
    //return 2 if we can move and there is a treasure
    //return 3 if we can move and there is a mine
    return 0;
}

int GameWindow::check() {
    //ask server if there something at this position
    //return 0 if there is nothing
    //return 1 if there is a treasure
    //return 2 if there is a mine
    return 0;
}

void GameWindow::drawWall(int curXCoor, int curYCoor, int direction) {

}

void GameWindow::shoot(string direction) {
    cerr << "shoot";
    if (isServer) {
        cerr << "shoot started\n";
        server->shoot(name, direction, 3);
    }
    //if killed someone then showKillText()
}

void GameWindow::dig() {
    if (isServer) {
        server->dig(name);
    }
    //+1 to players stash
    //remove treasure sector from this position
}

void GameWindow::showTreasureText() {
    //Shows treasureText();
}

void GameWindow::paintEvent(QPaintEvent *e) {
    QPainter painter(this);

    painter.drawLine(50, 100, 100, 100);
    painter.drawLine(75, 100, 75, 75);
    painter.drawLine(50, 100, 50, 75);
    painter.drawLine(100, 100, 100, 75);
    painter.drawLine(50, 75, 100, 75);
    painter.drawLine(65, 75, 65, 10);
    painter.drawLine(85, 75, 85, 10);
    painter.drawLine(65, 10, 85, 10);
    painter.drawLine(65, 20, 85, 20);
}
