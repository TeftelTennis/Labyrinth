
#include "menuwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.setWindowTitle("Some shit");
//    w.show();

//    MainWindow *w = new MainWindow();
//    w->setWindowTitle("Some shit");
//    w->show();

    MenuWindow menu;
    menu.exec();
    return a.exec();
}
