#include <deque>
#include <string>
#include <map>
#include "labyrinth.h"
#include "trap.h"
#include "labyrinthobjectfactory.h"
#include "serverdata.h"

#ifndef SERVER_H
#define SERVER_H

class Server {
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
    Server();

    //Вход выход игроков
    void killPlayer(string name);

    void OnPlayerDisconnected ();

    void addPlayer(int i, int j, string nameP);

    //Обрабатывает движение игрока
    void move(string nameP, string direct);

    void shoot(string nameP, string direct, int item);

    void dig(string nameP);

    void doTurn(string turn, string nameP);

    void startGame();

    void doCommand(string command);

    void OnGUI();

    void console(int windowID);
};


#endif // SERVER_H
