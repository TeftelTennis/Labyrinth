
#include "menuwindow.h"
#include "labyrinth.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MenuWindow menu;
    menu.exec();
    return a.exec();
}
