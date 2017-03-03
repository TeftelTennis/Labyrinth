#include <string>
#include <vector>
#include <functional>
#include "item.h"
#include "treasure.h"
#include "direction.h"
#include "player.h"
#include "labyrinthobject.h"
#include "dsu.h"
#include "labyrinthdata.h"
#include <memory>

#ifndef LABYRINTH_H
#define LABYRINTH_H

using namespace std;

class Labyrinth {
private:
    int const static MAXSIZE = 1000;
public:
    int w;
    int h;
    vector<LabyrinthObject*> cell[MAXSIZE][MAXSIZE];
    string horizontWalls[MAXSIZE][MAXSIZE];
    string verticalWalls[MAXSIZE][MAXSIZE];
    LabyrinthData data;

    Labyrinth(int w, int h);
    bool checkPos(int i, int j);
    //По кордам и направлению дает новые корды.
    pair<int, int> move(int i, int j, Direction direct);
    string getWall(int i, int j, Direction direct);
    void addWall(int i, int j, Direction direct, string wall);
    tuple<int, int, int> findPlayer(string name);
    void movePlayer(string name, Direction direct);
    void killPlayer(string name);
    void addObject(int i, int j, LabyrinthObject item) ;
    void makeBorder();
    bool wasWall[MAXSIZE][MAXSIZE][MAXSIZE];
    void create();
};




#endif // LABYRINTH_H
