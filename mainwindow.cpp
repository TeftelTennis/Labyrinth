#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma comment(lib, "opengl32.lib")

//Скорее всего удалю нахуй всю эту хуйню

MainWindow::MainWindow(QWidget *parent) :
    QGLWidget(parent)
{
    smth = 42;
    xCoor = 250;
    yCoor = 250;
    xSize = 500;
    ySize = 500;
    setFormat(QGLFormat(QGL::DoubleBuffer));
    glDepthFunc(GL_LEQUAL);

}

void MainWindow::initializeGL() {
    qglClearColor(Qt::blue);
}

void MainWindow::resizeGL(int nWidth, int nHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint) nWidth, (GLint) nHeight);
    xSize = nWidth;
    ySize = nHeight;
    xCoor = xSize / 2;
    yCoor = ySize / 2;
}

void MainWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, xSize, ySize, 0, 1, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    qglColor(Qt::white);
    renderText((double) xCoor, (double) yCoor, 0, (QString) smth, QFont());
}

void MainWindow::keyPressEvent(QKeyEvent *ke) {
    switch (ke->key()) {
        case Qt::Key_Space :
            smth += 1;
            break;
        case Qt::Key_Escape :
            parentWidget()->show();
            close();
            break;
    }
    updateGL();
}

MainWindow::~MainWindow()
{
}


