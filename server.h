#include <deque>
#include <string>
#include <map>
#include "labyrinth.h"
#include "trap.h"
#include "labyrinthobjectfactory.h"

#ifndef SERVER_H
#define SERVER_H

class Server {
    map<string, string> players;
    Labyrinth field;

    int startAmmo = 3;
    int startLife = 1;
    int maxPlayer = 100;

    deque<string> turnQueue;
    string turnPlayer;
    deque<string> tmpQueue;
    //ServerData serverData;
    LabyrinthObjectFactory objectFactory;



    bool isConsoleOpen = false;
    string command = "";
    vector<string> commandLog;
    pair<int, int> scrollPosition = make_pair(0, 0);
   bool consoleButtonPressed = false;


    void Start();

    void initField(serverData);

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

    void Update();

    void OnGUI();

    void console(int windowID);
};


#endif // SERVER_H
