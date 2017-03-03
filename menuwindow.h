#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QDialog>
#include "joinserver.h"
#include "hostserver.h"

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = 0);
    ~MenuWindow();

private slots:
    void on_joinButton_clicked();

    void on_hostButton_clicked();

private:
    Ui::MenuWindow *ui;
    JoinServer *join;
    HostServer *host;
};

#endif // MENUWINDOW_H
