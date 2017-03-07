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
    LabyrinthData data;

    ServerData();
    ServerData(int width, int height, int startAmmo,
               int startLife, int keys, int bullets, int mines, float wallProb, float staticTreasureProb, float loveToiletsProb,
               bool canPutTreasureTogether, bool useRandomTreasure);

};

#endif // SERVERDATA_H
