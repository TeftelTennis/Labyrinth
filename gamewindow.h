#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include "hostserver.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = 0);
    ~GameWindow();

    int width;
    int height;
    const int boxWidth = 15;
    const int wallWidth = 3;
    int summaryWidth;
    int summaryHeight;
    int xCoors;
    int yCoors;
    int startAmmo;
    int startLifes;
    int keys;
    int bullets;
    int mines;

    float wallProb;
    float staticTreasureProb;
    float loveToiletsProb;

    bool canPutTreasureTogether;
    bool useRandomTreasure;

    void setParams(int width, int height, int xCoors, int yCoors, int startAmmo,
                   int startLifes, int keys, int bullets, int mines);
    void setParamsFloat(float wallProb, float staticTreasureProb, float loveToiletsProb,
                        bool canPutTreasureTogether, bool useRandomTreasure);
    void keyPressEvent(QKeyEvent *key); //do smth depend on the key pressed
    void initialize(); //Drawing the start field, without any walls
    void update(); //update visual part????
    void move(int direction);
    int movePlayer(int direction); //Move player
    int check(); //Check if smth at this position, like treasure or landmine
    void drawWall(int curXCoor, int curYCoor, int direction);
    void showTreasureText();
    void shoot();
    void dig();

    void paintEvent(QPaintEvent *e); //show penis
private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
