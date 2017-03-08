#include "treasure.h"
#include <vector>

#ifndef LABYRINTHDATA_H
#define LABYRINTHDATA_H

using namespace std;

class LabyrinthData {
public:
    LabyrinthData();

    double wallProb;
    vector<Treasure> treasures;
    bool useRandomTreasure;
    int treasureCount;
    double loveToilets; // Если хочет ставить в сартр
    double staticTreasureProb;
    bool canPutTreasureTogether;


};

#endif // LABYRINTHDATA_H
