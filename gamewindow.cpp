#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
//    width = host.getWidth();
//    height = host.getHeight();
//    summaryWidth = width * boxWidth + (width - 1) * wallWidth;
//    summaryHeight = height * boxWidth + (height - 1) * wallWidth;

//    xCoors = host.getXCoor();
//    yCoors = host.getYCoor();

//    startAmmo = host.getStartAmmo();
//    startLifes = host.getStartLifes();
//    keys = host.getKeys();
//    bullets = host.getBullets();
//    mines = host.getMines();

//    wallProb = host.getWallProb();
//    staticTreasureProb = host.getStaticTreasureProb();
//    loveToiletsProb = host.getLoveToiletsProb();

//    canPutTreasureTogether = host.canPutTreasureTogether();
//    useRandomTreasure = host.useRandomTreasure();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::setParams(int width, int height, int xCoors, int yCoors, int startAmmo,
               int startLifes, int keys, int bullets, int mines) {
    this->width = width;
    this->height = height;
    summaryWidth = width * boxWidth + (width - 1) * wallWidth;
    summaryHeight = height * boxWidth + (height - 1) * wallWidth;

    this->xCoors = xCoors;
    this->yCoors = yCoors;
    this->startAmmo = startAmmo;
    this->startLifes = startLifes;
    this->keys = keys;
    this->bullets = bullets;
    this->mines = mines;
}

void GameWindow::setParamsFloat(float wallProb, float staticTreasureProb, float loveToiletsProb,
                    bool canPutTreasureTogether, bool useRandomTreasure) {
    this->wallProb = wallProb;
    this->staticTreasureProb = staticTreasureProb;
    this->loveToiletsProb = loveToiletsProb;
    this->canPutTreasureTogether = canPutTreasureTogether;
    this->useRandomTreasure = useRandomTreasure;
}

void GameWindow::keyPressEvent(QKeyEvent *key) {
    switch (key->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Up:
            move(1);
            break;
        case Qt::Key_Right:
            move(2);
            break;
        case Qt::Key_Left:
            move(0);
            break;
        case Qt::Key_Down:
            move(3);
            break;
        case Qt::Key_D:
            shoot();
            break;
        case Qt::Key_A:
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

void GameWindow::move(int direction) {
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
    }
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

void GameWindow::shoot() {
    //if killed someone then showKillText()
}

void GameWindow::dig() {
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
