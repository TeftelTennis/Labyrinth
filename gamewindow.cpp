#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <iostream>

using namespace std;

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    widg = new QWidget(this);
    area = ui->scrollArea;
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    layout = new QVBoxLayout(widg);
    area->setWidget(widg);
    area->setWidgetResizable(true);

    cerr << "created";

}

GameWindow::~GameWindow()
{
    delete ui;
}


int GameWindow::getPosFromXCoors(int x) {
    return 10 + (x - 1) * (boxWidth + wallWidth);
}

int GameWindow::getPosFromYCoors(int y) {
    return 10 + (y - 1) * (boxWidth + wallWidth);
}

void GameWindow::drawField(GameLog *gamelog) {
    if (gamelog->player.name == this->name) {
        isMyWindow = true;
        drawMyWindow(gamelog);
    } else {
        isMyWindow = false;
        drawEnemy(gamelog);
    }
}

void GameWindow::addLog(string s) {
    QLabel *label = new QLabel(QString::fromStdString(s));
    layout->addWidget(label);
    area->verticalScrollBar()->setSliderPosition(area->height());
}

void GameWindow::drawLines(int width, int height, int sumWidth, int sumHeight) {
    QPen myPen(Qt::black);
    myPen.setWidth(5);
    for (int i = 1; i < width; i++) {
        int x = 10 + i * (boxWidth + wallWidth);
        QLineF line(x - wallWidth, 10, x - wallWidth, 10 + sumHeight);
        QLineF line2(x, 10, x, 10 + sumHeight);
        scene->addLine(line, myPen);
        scene->addLine(line2, myPen);
    }

    for (int i = 1; i < height; i++) {
        int y = 10 + i * (boxWidth + wallWidth);
        QLineF line(10, y - wallWidth, 10 + sumWidth, y - wallWidth);
        QLineF line2(10, y, 10 + sumWidth, y);
        scene->addLine(line, myPen);
        scene->addLine(line2, myPen);
    }
}

void GameWindow::drawMyWindow(GameLog *gamelog) {
    if (winner != "null") {
        scene->clear();
        scene->addText(QString::fromStdString(winner + " WON THE GAME!"), QFont("Times", 13, QFont::Bold));
        this->resize(500, 500);
        return;
    }
    if (gamelog->player.life == 0) {
        scene->clear();
        scene->addText("Game Over.", QFont("Times", 13, QFont::Bold));
        this->resize(500, 500);
        return;
    }
    QBrush brush = QBrush(0xcfbea5);
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);
    scene->addRect(10, 10, summaryWidth, summaryHeight, myPen, brush);
    drawMenu();
    this->resize(summaryWidth + 600, std::max(summaryHeight, 500) + 50);
    drawLines(width, height, summaryWidth, summaryHeight);

    drawHorizontalWalls(gamelog);
    drawVerticalWalls(gamelog);

    drawPath(gamelog->turn, gamelog->jStart + 1, height - gamelog->iStart);
}

void GameWindow::drawEnemy(GameLog *gamelog) {
    if (winner != "null") {
        scene->clear();
        scene->addText(QString::fromStdString(winner + " WON THE GAME!"), QFont("Times", 13, QFont::Bold));
        this->resize(500, 500);
        return;
    }
    int thisWidth = width * 2 - 1;
    int thisHeight = height * 2 - 1;
    int thisSummaryWidth = thisWidth * boxWidth + (thisWidth - 1) * wallWidth;
    int thisSummaryHeight = thisHeight * boxWidth + (thisHeight - 1) * wallWidth;

    QBrush brush = QBrush(0xcfbea5);
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);
    scene->addRect(10, 10, thisSummaryWidth, thisSummaryHeight, myPen, brush);

    this->resize(thisSummaryWidth + 400, thisSummaryHeight + 100);
    cerr << "player has " << gamelog->player.life << " lifes\n";
    QString playerName = QString::fromStdString(gamelog->player.name);
    if (gamelog->player.life <= 0) {
        playerName += " DEAD";
    }
    QGraphicsTextItem *plname = scene->addText(playerName, QFont("Times", 13, QFont::Bold));
    plname->setPos(thisSummaryWidth / 2, thisSummaryHeight + 40);
    drawLines(thisWidth, thisHeight, thisSummaryWidth, thisSummaryHeight);

    drawHorizontalWalls(gamelog);
    drawVerticalWalls(gamelog);

    drawPath(gamelog->turn, width, height);

}

void GameWindow::drawLine(int xFrom, int yFrom, string direction) {
    int xStart = getPosFromXCoors(xFrom) + 25;
    int yStart = getPosFromYCoors(yFrom) + 25;
    int xFin = xStart;
    int yFin = yStart;
    if (direction == "up") {
        yFin -= 60;
    } else if (direction == "left") {
        xFin -= 60;
    } else if (direction == "right") {
        xFin += 60;
    } else {
        yFin += 60;
    }
    QLineF line(xStart, yStart, xFin, yFin);
    scene->addLine(line, QPen(Qt::red));
}

void GameWindow::drawPath(vector<Direction> directions, int x, int y) {
    //ну типа рисуем линии начиная со стартовой позиции x y
    int xx = x;
    int yy = y;
    for (auto direction : directions) {
        switch (direction.dir) {
            case 0:
                drawLine(xx, yy--, "up");
                break;
            case 1:
                drawLine(xx--, yy, "left");
                break;
            case 2:
                drawLine(xx, yy++, "down");
                break;
            case 3:
                drawLine(xx++, yy, "right");
                break;
        }
    }
    playerIcon = scene->addEllipse(getPosFromXCoors(xx) + 5,
                                   getPosFromYCoors(yy) + 5,
                                   40, 40, QPen(Qt::black), QBrush(Qt::blue));

}
void GameWindow::sendtoserver(string data) {
    if (tcpSocket->waitForConnected(1000)) {
       QString string = QString::fromStdString(data);
        QByteArray array;
        array.append(string);
        qDebug()<<  tcpSocket->write(array);
    }
}


void GameWindow::startJoin(int x, int y, string name) {
    tcpSocket = new QTcpSocket(this);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    this->name = name;
    isServer = false;
    isMyWindow = true;
    isInitialised = false;
    isTreasure = false;
    logPosition = 0;
    winner = "null";
    nameNext = "null";

    xCoors = x;
    yCoors = y;
    tcpSocket->connectToHost(QHostAddress::LocalHost, 33333);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataFromServer()));
    string qstr = "new " + to_string(x) + " " + to_string(y) + " " + name;
    sendtoserver(qstr);
}

void GameWindow::readDataFromServer() {
    QTcpSocket* serverSocket = (QTcpSocket*)sender();
    QByteArray data = serverSocket->readAll();
    string sdata = data.toStdString();
    cerr << "frome server : " << sdata<<'\n';
    doResultOfTurn(sdata);

}

void GameWindow::setServerParams(string name, int x, int y, ServerData serverData) {

    cerr << "paramsstart";
    this->name = name;
    this->nameNext = name;

    this->isServer = true;
    this->isMyWindow = true;
    this->isInitialised = true;
    this->isTreasure = false;
    server = new Server(serverData);
    winner = "null";


    server->addPlayer(x, y, name);
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::LocalHost, 33333) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
    } else {
        server_status=1;
        cerr << "started";
        qDebug() << tcpServer->serverAddress() << " " << tcpServer->serverPort() << " adress and port\n";
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }

    summaryWidth = serverData.width * boxWidth + (serverData.width - 1) * wallWidth;
    summaryHeight = serverData.height * boxWidth + (serverData.height - 1) * wallWidth;
    this->resize(summaryWidth + 600, std::max(summaryHeight, 500) + 50);
    xCoors = x + 1;
    yCoors = serverData.height - y;
    keys = 0;
    bullets = serverData.startAmmo;
    life = serverData.startLife;
    width = serverData.width;
    height = serverData.height;
    gamelogs.push_back(GameLog(this->name, width, height, bullets, 1, true, x, y));
    logPosition = 0;
    update();
}


void GameWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void GameWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    QByteArray data = clientSocket->readAll();
    qDebug() << data << "\n";
    string sdata = data.toStdString();
    string result = server->doTurn(sdata);
    sendtoall(result);
    doResultOfTurn(result);
}

void GameWindow::sendtoall(string msg) {

    QString serverdata = QString::fromStdString(msg);
    QByteArray array;
    array.append(serverdata);
    foreach (QTcpSocket* iter, SClients) {
        qDebug() << iter->write(array) << "\n";
    }
}

void GameWindow::keyPressEvent(QKeyEvent *key) {
    switch (key->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_T:
            if (logPosition != 0) {
                scene->clear();
                drawField(&gamelogs[--logPosition]);
            }
            break;
        case Qt::Key_Y:
            if (logPosition != static_cast<int>(gamelogs.size() - 1)) {
                scene->clear();
                drawField(&gamelogs[++logPosition]);
            }
            break;
        case Qt::Key_W:
            if (isMyWindow && (nameNext == name)) move("up");
            break;
        case Qt::Key_A:
            if (isMyWindow && (nameNext == name)) move("left");
            break;
        case Qt::Key_D:
            if (isMyWindow && (nameNext == name)) {
                move("right");
            }
            break;
        case Qt::Key_S:
            if (isMyWindow && (nameNext == name)) move("down");
            break;
        case Qt::Key_I:
            if (isMyWindow && (nameNext == name)) shoot("up");
            break;
        case Qt::Key_J:
            if (isMyWindow && (nameNext == name)) shoot("left");
            break;
        case Qt::Key_K:
            if (isMyWindow && (nameNext == name)) shoot("down");
            break;
        case Qt::Key_L:
            if (isMyWindow && (nameNext == name)) shoot("right");
            break;
        case Qt::Key_Q:
            if (isMyWindow && (nameNext == name)) dig();
            break;
    }
}
void GameWindow::drawMenu() {
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);
    menu = scene->addRect(summaryWidth + 50, 10, 200, 500, myPen, QBrush(Qt::gray));

    controls = scene->addText("Controls: ", QFont("Times", 16, QFont::Bold));
    wsad = scene->addText("WSAD -- move");
    sh = scene->addText("IKJL -- shoot (WSAD)");
    dg = scene->addText("Q -- dig");
    inventory = scene->addText("Inventory", QFont("Times", 16, QFont::Bold));
    QGraphicsTextItem *keyText = scene->addText("Keys", QFont("Times", 13, QFont::Bold));
    QGraphicsTextItem *bulletText = scene->addText("Bullets", QFont("Times", 13, QFont::Bold));
    QFont font("Times", 16, QFont::Bold);
    treasureText = scene->addText("TREASURE!", font);
    treasureText->setDefaultTextColor(Qt::red);
    keyNum = scene->addText(QString::number(gamelogs[logPosition].player.keys()), QFont("Times", 16));
    bulletNum = scene->addText(QString::number(gamelogs[logPosition].player.ammo()), QFont("Times", 16));

    controls->setPos(100 + summaryWidth, 20);
    inventory->setPos(100 + summaryWidth, 120);
    wsad->setPos(90 + summaryWidth, 50);
    sh->setPos(90 + summaryWidth, 70);
    dg->setPos(90 + summaryWidth, 90);
    keyText->setPos(110 + summaryWidth, 170);
    bulletText->setPos(110 + summaryWidth, 220);
    treasureText->setPos(90 + summaryWidth, 400);
    keyNum->setPos(197 + summaryWidth, 170);
    bulletNum->setPos(197 + summaryWidth, 220);

    for (int i = 0; i < 3; i++) {
        QLineF line(185 + summaryWidth, 160 + i * 50, 235 + summaryWidth, 160 + i * 50);
        scene->addLine(line, QPen(Qt::black));
    }

    for (int i = 0; i < 2; i++) {
        QLineF line(185 + summaryWidth + i * 50, 160, 185 + summaryWidth + i * 50, 260);
        scene->addLine(line, QPen(Qt::black));
    }
    if (logPosition == 0 && !isTreasure) {
        hideTreasureText();
    }
    if (logPosition != 0) {
        hideTreasureText();
    }
}



void GameWindow::update() {
    drawField(&gamelogs[logPosition]);
}


void GameWindow::move(string direction) {
    if (isServer) {
        string result = server->move(name, direction);
        sendtoall(result);
        doResultOfTurn(result);

    }
    else {
        string send = name + " move " + direction;
        sendtoserver(send);
    }
    update();
    return;
}


void GameWindow::shoot(string direction) {
    cerr << "shoot";
    if (isServer) {
        string result = server->shoot(name, direction, 3);
        sendtoall(result);
        doResultOfTurn(result);

    }
    else {
        string send = name + " shoot " + direction + " 3";
        sendtoserver(send);
    }
}

void GameWindow::dig() {
    if (isServer) {
        string result = server->dig(name);
        sendtoall(result);
        doResultOfTurn(result);

    }
    else {
        string send = name + " dig";
        sendtoserver(send);
    }
    hideTreasureText();
}


void GameWindow::doResultOfTurn(string turnn) {

    if(logPosition == 0) {
        isTreasure = false;
    }

    if ((turnn[0] == 'd')) {
        if (isInitialised) {
            return;
        }
        vector<string> parsed = splitter::split(' ', 6, turnn);
        for (auto q: parsed) {
            cerr << q << "%";
        }
        summaryWidth = stoi(parsed[1]) * boxWidth + (stoi(parsed[1]) - 1) * wallWidth;
        summaryHeight = stoi(parsed[2]) * boxWidth + (stoi(parsed[2]) - 1) * wallWidth;
        this->resize(summaryWidth + 600, std::max(summaryHeight, 500) + 50);
        keys = 0;
        bullets = stoi(parsed[3]);
        width = stoi(parsed[1]);
        height = stoi(parsed[2]);
        life = stoi(parsed[4]);
        gamelogs.push_back(GameLog(name, width, height, bullets, life, true, xCoors, yCoors));
        xCoors += 1;
        yCoors = stoi(parsed[2]) - yCoors;
        isInitialised = true;
        update();
        return;
    }
    vector<string> turn = splitter::split(' ', 100, turnn);
    string nameP = turn[0];
    nameNext = turn[turn.size() - 1];
    int curlog = -1;
    for (int i = 0; i < static_cast<int>(gamelogs.size()); i++) {
        if (gamelogs[i].player.name == nameP) {
            curlog = i;
        }
    }
    if (curlog == -1) {
        gamelogs.push_back(GameLog(nameP, 2 * width - 1, 2 * height - 1, bullets, life, false, width - 1, height - 1));
        curlog = gamelogs.size() - 1;
    }
    string tolog = nameP + " ";
    if (turn[1] == "move") {
        if (turn[3] == "win") {
            winner = nameP;
        }
        if (turn[3] == "wall") {
            tolog += "cannot move to " + turn[2] + " because of wall";
            gamelogs[curlog].addWall(Direction(turn[2]), "wall");
        } else if (turn[3] == "move") {
            tolog += "moves to " + turn[2];
            gamelogs[curlog].addMove(Direction(turn[2]));
            int corpseCount = stoi(turn[4]);
            if (corpseCount != 0) {
                tolog += " there are " + turn[4] + " corpses: ";
            }
            for (int it = 0; it < corpseCount; it++) {
                string name  = turn[5 + it];
                tolog += name + " ";
            }
            int trapCount  = stoi(turn[5 + corpseCount]);
            if (corpseCount != 0) {
                tolog += " there are " + turn[5 + corpseCount] + " traps";
            }
            if (turn[6 + corpseCount + trapCount] == "1") {
                tolog += " and founds a SECRET place";
                if (nameP == name) {
                    isTreasure = true;
                }
            }
        }
    } else if (turn[1] == "shoot") {
        gamelogs[logPosition].player.deleteItem(Bullet());
        int victimCount = stoi(turn[4]);
        tolog += "shoots " + turn[2] + " and hits " + turn[4] + " target(s): ";
        bool isSamePos = turn[victimCount + 5] == "1";
        for (int it = 0; it < victimCount; it++) {
            string nameHit = turn[5 + it];
            tolog += nameHit + " is injured ";
            int hitlog = -1;
            for (int i = 0; i < static_cast<int>(gamelogs.size()); i++) {
                if (gamelogs[i].player.name == nameHit) {
                    hitlog = i;
                    gamelogs[i].player.life--;
                    break;
                }
            }
            if (hitlog == -1) {
                gamelogs.push_back(GameLog(nameHit, 2 * width - 1, 2 * height - 1, bullets, life - 1, false, width - 1, height - 1));
                hitlog = gamelogs.size() - 1;
            }
            if (isSamePos && (gamelogs[hitlog].player.life == 0)) {
                tolog += "and you took his items";
            }
        }
    } else if (turn[1] == "dig") {
        int count = stoi(turn[2]);
        tolog += " digs " + turn[2] + " items:";
        for (int iter = 0; iter < count; iter++) {
            if (stoi(turn[3 + 2 * iter]) == 0) {
                if (turn[4 + 2 * iter] == "4") {
                    tolog += " key";
                    gamelogs[curlog].player.items.push_back(new Key());
                }
                else {
                    tolog += " bullet";
                    gamelogs[curlog].player.items.push_back(new Bullet());
                }
            }
        }
    }
    logs.push_back(tolog);
    addLog(tolog);
    update();
}



void GameWindow::drawHorizontalWalls(GameLog *gamelog) {
    for (int i = 0; i < gamelog->h + 1; i++) {
        for (int j = 0; j < gamelog->w; j++) {
            if (gamelog->horizontWalls[i][j] == "wall") {
                drawWall(j + 1, gamelog->h - i, 2);
            }
        }
    }
}

void GameWindow::drawVerticalWalls(GameLog *gamelog) {
    for (int i = 0; i < gamelog->h; i++) {
        for (int j = 0; j < gamelog->w + 1; j++) {
            if (gamelog->verticalWalls[i][j] == "wall") {
                drawWall(j + 1, gamelog->h - i, 1);
            }
        }
    }
}

void GameWindow::drawWall(int curXCoor, int curYCoor, int direction) {
    QPen pen = QPen(Qt::black);
    pen.setWidth(10);
    int x1;
    int y1;
    int x2;
    int y2;
    switch (direction) {
        case 1:
            x1 = getPosFromXCoors(curXCoor) - 5;
            y1 = getPosFromYCoors(curYCoor) - 5;
            x2 = x1;
            y2 = y1 + 60;
            break;
        case 0:
            x1 = getPosFromXCoors(curXCoor) - 5;
            y1 = getPosFromYCoors(curYCoor) - 5;
            x2 = x1 + 60;
            y2 = y1;
            break;
        case 3:
            x1 = getPosFromXCoors(curXCoor) + 55;
            y1 = getPosFromYCoors(curYCoor) - 5;
            x2 = x1;
            y2 = y1 + 60;
            break;
        case 2:
            x1 = getPosFromXCoors(curXCoor) - 5;
            y1 = getPosFromYCoors(curYCoor) + 55;
            x2 = x1 + 60;
            y2 = y1;
            break;
    }
    QLineF line(x1, y1, x2, y2);
    scene->addLine(line, pen);
}

void GameWindow::showTreasureText() {
    treasureText->show();
}

void GameWindow::hideTreasureText() {
    treasureText->hide();
}
