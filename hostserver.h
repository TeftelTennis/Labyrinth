#ifndef HOSTSERVER_H
#define HOSTSERVER_H

#include <QDialog>
#include "gamewindow.h"

namespace Ui {
class HostServer;
}

class HostServer : public QDialog
{
    Q_OBJECT

public:
    explicit HostServer(QWidget *parent = 0);
    ~HostServer();

private slots:
    void on_menuButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::HostServer *ui;
};

#endif // HOSTSERVER_H
