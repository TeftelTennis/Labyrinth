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
    cerr << "created";

}

GameWindow::~GameWindow()
{
    delete ui;
}

int GameWindow::getPosFromXCoor() {
    return 10 + (xCoors - 1) * (boxWidth + wallWidth);
}

int GameWindow::getPosFromYCoor() {
    return 10 + (yCoors - 1) * (boxWidth + wallWidth);
}

//сорян за копипасту ебаную, вдруг просто что-то полетит еще, потом подотру
int GameWindow::getPosFromXCoors(int x) {
    return 10 + (x - 1) * (boxWidth + wallWidth);
}

int GameWindow::getPosFromYCoors(int y) {
    return 10 + (y - 1) * (boxWidth + wallWidth);
}

void GameWindow::drawField(GameLog *gamelog) {
    if (gamelog->player.name == this->name) {
        drawMyWindow(gamelog);
    } else {
        drawEnemy(gamelog);
    }
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
    QBrush brush = QBrush(0xcfbea5);
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);
    scene->addRect(10, 10, summaryWidth, summaryHeight, myPen, brush);
    drawMenu();
    this->resize(summaryWidth + 300, std::max(summaryHeight, 500) + 50);
    drawLines(width, height, summaryWidth, summaryHeight);

    drawHorizontalWalls(gamelog, true);
    drawVerticalWalls(gamelog, true);

    drawPath(gamelog->turn, gamelog->iStart, gamelog->jStart);
    playerIcon = scene->addEllipse(getPosFromXCoors(gamelog->iCur) + 5,
                                   getPosFromYCoors(gamelog->jCur) + 5,
                                   40, 40, QPen(Qt::black), QBrush(Qt::blue));
}

void GameWindow::drawEnemy(GameLog *gamelog) {
    int thisWidth = width * 2 - 1;
    int thisHeight = height * 2 - 1;
    int thisSummaryWidth = thisWidth * boxWidth + (thisWidth - 1) * wallWidth;
    int thisSummaryHeight = thisHeight * boxWidth + (thisHeight - 1) * wallWidth;

    QBrush brush = QBrush(0xcfbea5);
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);
    scene->addRect(10, 10, thisSummaryWidth, thisSummaryHeight, myPen, brush);

    this->resize(thisSummaryWidth + 100, thisSummaryHeight + 100);
    QString playerName = QString::fromStdString(gamelog->player.name);
    QGraphicsTextItem *plname = scene->addText(playerName, QFont("Times", 13, QFont::Bold));
    plname->setPos(thisSummaryWidth / 2, thisSummaryHeight + 40);
    drawLines(thisWidth, thisHeight, thisSummaryWidth, thisSummaryHeight);

    drawHorizontalWalls(gamelog, false);
    drawVerticalWalls(gamelog, false);

    drawPath(gamelog->turn, width - 1, height - 1);

    playerIcon = scene->addEllipse(getPosFromXCoors(width) + 5,
                                   getPosFromYCoors(height) + 5,
                                   40, 40, QPen(Qt::black), QBrush(Qt::blue));

}

void GameWindow::drawPath(vector<Direction> directions, int x, int y) {
    //ну типа рисуем линии начиная со стартовой позиции x y
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
    cerr << sdata<<'\n';
    doResultOfTurn(sdata);

}

void GameWindow::setServerParams(string name, int x, int y, ServerData serverData) {

    cerr << "paramsstart";
    this->name = name;

    this->isServer = true;
    server = new Server(serverData);

    server->addPlayer(x, y, name);
    server->addPlayer(0, 1, "nigga");
    tcpServer = new QTcpServer(this); //было this
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
    this->resize(summaryWidth + 300, std::max(summaryHeight, 500) + 50);
    xCoors = x + 1;
    yCoors = serverData.height - y;
    keys = 0;
    bullets = serverData.startAmmo;
    width = serverData.width;
    height = serverData.height;
    gamelogs.push_back(GameLog(this->name, width, height, bullets, 1, true, x, y));
    gamelogs.push_back(GameLog("sosna", width, height, bullets, 2, true, 1, 2));
    logPosition = 0;
    initialize();
}


void GameWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
        //string qwe = "data " + to_string(server->field->w) + " " +  to_string(server->field->h) + " "
        //    + to_string(server->serverData.startAmmo) + " " + to_string(server->serverData.startLife);

        //QString serverdata = QString::fromStdString(qwe);
        //QByteArray array;
        //array.append(serverdata);

        //qDebug() << clientSocket->write(array);

    }
}

void GameWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    QByteArray data = clientSocket->readAll();
    qDebug() << data;
    string sdata = data.toStdString();
    string result = server->doTurn(sdata);
    QString serverdata = QString::fromStdString(result);
    QByteArray array;
    array.append(serverdata);

    qDebug() << clientSocket->write(array);
}


void GameWindow::keyPressEvent(QKeyEvent *key) {
    switch (key->key()) {
        case Qt::Key_Escape:
            close();
            break;
            //Нужен какой-то счетчик-итератор, типа знать где мы находимся
        case Qt::Key_T:
            if (logPosition != 0) {
                scene->clear();
                drawField(&gamelogs[--logPosition]);
            }
            break;
        case Qt::Key_Y:
            if (logPosition != gamelogs.size() - 1) {
                scene->clear();
                drawField(&gamelogs[++logPosition]);
            }
            break;
        case Qt::Key_W:
            move("up");
            break;
        case Qt::Key_A:
            move("left");
            break;
        case Qt::Key_D:
            move("right");
            break;
        case Qt::Key_S:
            move("down");
            break;
        case Qt::Key_I:
            shoot("up");
            break;
        case Qt::Key_J:
            shoot("left");
            break;
        case Qt::Key_K:
            shoot("down");
            break;
        case Qt::Key_L:
            shoot("right");
            break;
        case Qt::Key_Q:
            dig();
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
    keyNum = scene->addText(QString::number(keys), QFont("Times", 16));
    bulletNum = scene->addText(QString::number(bullets), QFont("Times", 16));

    controls->setPos(100 + summaryWidth, 20);
    inventory->setPos(100 + summaryWidth, 120);
    wsad->setPos(90 + summaryWidth, 50);
    sh->setPos(90 + summaryWidth, 70);
    dg->setPos(90 + summaryWidth, 90);
    keyText->setPos(110 + summaryWidth, 170);
    bulletText->setPos(110 + summaryWidth, 220);
    treasureText->setPos(90 + summaryWidth, 400);
    treasureText->hide();
    keyNum->setPos(197 + summaryWidth, 170);
    bulletNum->setPos(197 + summaryWidth, 220);
}

void GameWindow::initialize() {
    //creates a new field for current player
    //menu AlignRight with keys description
    QBrush blueBrush = QBrush(Qt::blue);
    QBrush brush = QBrush(0xcfbea5);

    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);

    scene->addRect(10, 10, summaryWidth, summaryHeight, myPen, brush);
    drawMenu();

    for (int i = 0; i < 3; i++) {
        QLineF line(185 + summaryWidth, 160 + i * 50, 235 + summaryWidth, 160 + i * 50);
        scene->addLine(line, QPen(Qt::black));
    }

    for (int i = 0; i < 2; i++) {
        QLineF line(185 + summaryWidth + i * 50, 160, 185 + summaryWidth + i * 50, 260);
        scene->addLine(line, QPen(Qt::black));
    }

    for (int i = 1; i < width; i++) {
        int x = 10 + i * (boxWidth + wallWidth);
        QLineF line(x - wallWidth, 10, x - wallWidth, 10 + summaryHeight);
        QLineF line2(x, 10, x, 10 + summaryHeight);
        scene->addLine(line, myPen);
        scene->addLine(line2, myPen);
    }

    for (int i = 1; i < height; i++) {
        int y = 10 + i * (boxWidth + wallWidth);
        QLineF line(10, y - wallWidth, 10 + summaryWidth, y - wallWidth);
        QLineF line2(10, y, 10 + summaryWidth, y);
        scene->addLine(line, myPen);
        scene->addLine(line2, myPen);
    }
    myPen.setWidth(3);
    playerIcon = scene->addEllipse(getPosFromXCoor() + 5, getPosFromYCoor() + 5, 40, 40, myPen, blueBrush);

}

void GameWindow::update() {
    playerXCoor = getPosFromXCoor();
    playerYCoor = getPosFromYCoor();
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(3);
    QBrush blueBrush = QBrush(Qt::blue);
    scene->removeItem((QGraphicsItem*) playerIcon);
    playerIcon = scene->addEllipse(playerXCoor + 5, playerYCoor + 5, 40, 40, myPen, blueBrush);
}

void GameWindow::updateInfo() {
    scene->removeItem((QGraphicsItem*) keyNum);
    scene->removeItem((QGraphicsItem*) bulletNum);
    keyNum = scene->addText(QString::number(keys), QFont("Times", 16));
    bulletNum = scene->addText(QString::number(bullets), QFont("Times", 16));
//    if (check() == 1) {
//        showTreasureText();
//    } else {
//        hideTreasureText();
//    }
}

void GameWindow::move(string direction) {
    int i = movePlayer(direction);
    int dir;
    if (direction == "left") {
        dir = 1;
    } else if (direction == "up") {
        dir = 0;
    } else if (direction == "right") {
        dir = 3;
    } else {
        dir = 2;
    }
    switch (i) {
        case 0:
            switch (dir) {
            case 0:
                yCoors--;
                break;
            case 1:
                xCoors--;
                break;
            case 2:
                yCoors++;
                break;
            case 3:
                xCoors++;
                break;
            }

            update();
            break;
        case 1:
            drawWall(xCoors, yCoors, dir);
            break;
        case 2:
            switch (dir) {
            case 0:
                yCoors--;
                break;
            case 1:
                xCoors--;
                break;
            case 2:
                yCoors++;
                break;
            case 3:
                xCoors++;
                break;
            }

            update();
            showTreasureText();
            break;
        case 3:
            //player life's -1 and checkIfDead()
            break;
    }
}

int GameWindow::movePlayer(string direction) { //direction: 0 - up, 1 - left, 2 - down, 3 - right
    //return 0 if player can get to that direction
    //return 1 if there is a wall
    //return 2 if we can move and there is a treasure
    //return 3 if we can move and there is a mine
    if (isServer) {
        string result = server->move(name, direction);
        //sendtoall(result);
        vector<string> parsed = splitter::split(' ', 5, result);
        cerr << "LOOK AT THIS: " << result << endl;
        cerr << parsed[3] << ' ' << parsed[4] << endl;
        if (parsed[3] == "wall") return 1;
    }
    else {
        string send = name + " move " + direction;
        sendtoserver(send);
    }
    return 0;
}

void GameWindow::shoot(string direction) {
    cerr << "shoot";
    if (isServer) {
        //cerr << "shoot started\n";
        string result = server->shoot(name, direction, 3);
        //sendtoall(result);
    }
    else {
        string send = name + " shoot " + direction + " 3";
        sendtoserver(send);
    }
    //if killed someone then showKillText()
}

void GameWindow::dig() {
    if (isServer) {
        string result = server->dig(name);
        //sendtoall(result)
    }
    else {
        string send = name + " dig";
        sendtoserver(send);
    }
    hideTreasureText();
    //+1 to players stash
    //remove treasure sector from this position
}


void GameWindow::doResultOfTurn(string turnn) {
    if (turnn[0] == 'd') {
        vector<string> parsed = splitter::split(' ', 5, turnn);
        for (auto q: parsed) {
            cerr << q << "%";
        }
        summaryWidth = stoi(parsed[1]) * boxWidth + (stoi(parsed[1]) - 1) * wallWidth;
        summaryHeight = stoi(parsed[2]) * boxWidth + (stoi(parsed[2]) - 1) * wallWidth;
        this->resize(summaryWidth + 300, std::max(summaryHeight, 500) + 50);
        xCoors += 1;
        yCoors = stoi(parsed[2]) - yCoors;
        keys = 0;
        bullets = stoi(parsed[3]);
        width = stoi(parsed[1]);
        height = stoi(parsed[2]);
        initialize();
        return;
    }
    vector<string> turn = splitter::split(' ', 100, turnn);
    string nameP = turn[0];
    int curlog;
    for (int i = 0; i < gamelogs.size(); i++) {
        if (gamelogs[i].player.name == nameP) {
            curlog = i;
        }
    }
    string tolog = nameP;
    if (turn[1] == "move") {
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
            }
        }
    } else if (turn[1] == "shoot") {
        int victimCount = stoi(turn[4]);
        tolog += "shoots " + turn[2] + " and hits " + turn[4] + " target(s): ";
        bool isSamePos = turn[victimCount + 5] == "1";
        for (int it = 0; it < victimCount; it++) {
            string name = turn[5 + it];
            tolog += name + "is injured ";
            if (isSamePos) {
                tolog += "and you took his items";
            }
        }
    } else if (turn[1] == "dig") {
        int count = stoi(turn[2]);
        tolog += " digs " + turn[2] + " items";
    }
    logs.push_back(tolog);
}



void GameWindow::drawHorizontalWalls(GameLog *gamelog, bool isMine) {
    for (int i = 0; i < height + 1; i++) {
        for (int j = 0; j < width; j++) {
            if (gamelog->horizontWalls[i][j] == "wall") {
                if (isMine) {
                    drawWall(j, i, 3);
                } else {
                    drawWall(j + width - gamelog->jStart - 1,
                             i + height - gamelog->iStart - 1, 3);
                }
            }
        }
    }
}

void GameWindow::drawVerticalWalls(GameLog *gamelog, bool isMine) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width + 1; j++) {
            if (gamelog->verticalWalls[i][j] == "wall") {
                if (isMine) {
                    drawWall(j, i, 0);
                } else {
                    drawWall(j + width - gamelog->jStart - 1,
                             i + height - gamelog->iStart - 1, 0);
                }
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


void sendtoall(string msg){}
void sendtoserver(string msg){}
