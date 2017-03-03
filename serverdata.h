#include "labyrinthdata.h"
#ifndef SERVERDATA_H
#define SERVERDATA_H


class ServerData {
public:
    int startAmmo;
    int startLife;
    int maxPlayers;

    int height;
    int width;
    int maxSize = 30;
    int maxCnt = 20;
    int minSize = 1;
    LabyrinthData data;

    void Start();

    //function serverSettingsWindow();     //TODO окно для настроек
        //числа: width, height, startAmmo, startLife, treasureCount
        //флоаты от 0 до 1: количество стен wallProb, вероятность сокровищ в тупиках loveToilets, распределение сокровищ staticTreasureProb
        //булы: можно ли ставить сокровища в одну клетку canPutTreasureTogether, рандомные ли сокровища useRandomTreasure

};

#endif // SERVERDATA_H
