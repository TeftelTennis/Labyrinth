#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QtCore>

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
protected:
    void keyPressEvent(QKeyEvent *key); //do smth depend on the key pressed
    void initialize(); //Drawing the start field, without any walls
    void update(); //update visual part????
    void movePlayer(double xFrom, double yFrom, double xTo, double yTo); //Move player
    void check(); //Check if smth at this position, like treasure, or landmine
    void paintEvent(QPaintEvent *e); //show penis
private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
