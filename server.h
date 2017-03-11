#include <deque>
#include <string>
#include <map>
#include "labyrinth.h"
#include "trap.h"
#include "labyrinthobjectfactory.h"
#include "serverdata.h"

#ifndef SERVER_H
#define SERVER_H

class Server : public QDialog{
public:
    map<string, string> players;
    Labyrinth* field;

    int startAmmo = 3;
    int startLife = 1;
    int maxPlayer = 100;

    deque<string> turnQueue;
    string turnPlayer;
    deque<string> tmpQueue;
    ServerData serverData;
    LabyrinthObjectFactory objectFactory;

    string command = "";
    vector<string> commandLog;
    bool consoleButtonPressed = false;

    Server(ServerData serverData);

    //Вход игроков
    void killPlayer(string name);

    void addPlayer(int i, int j, string nameP);

    //Обрабатывает движение игрока
    string move(string nameP, string direct);

    string shoot(string nameP, string direct, int item);

    string dig(string nameP);

    string doTurn(string turn);

    void startGame();

};


#endif // SERVER_H
