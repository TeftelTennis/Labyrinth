#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>

//Это тоже все к хуям

namespace Ui {
class MainWindow;
}

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    int smth;
    int xCoor;
    int yCoor;
    int xSize;
    int ySize;

    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void keyPressEvent(QKeyEvent *ke);
};

#endif // MAINWINDOW_H
