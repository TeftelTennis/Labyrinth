#include <string>
#include <vector>
#include <functional>
#include "item.h"
#include "treasure.h"

#ifndef LABYRINTH_H
#define LABYRINTH_H

using namespace std;

class Direction {
public:
    int UP = 0;
    int LEFT = 1;
    int DOWN = 2;
    int RIGHT = 3;
    string name[4] = {"up", "left", "down", "right"};

    int dir;
    string getName();
    Direction();
    Direction(int a);
    Direction(string a);
    void turnClockwise();
    void turnCounterClockwise();
    void reverse();
};

class LabyrinthObject {
public:
    int const static TYPE_EMPTY = 0;
    int const static TYPE_ITEM = 1;
    int const static TYPE_PLAYER = 2;
    int const static TYPE_TRAP = 3;
    int const static TYPE_TREASURE = 4;

    int i, j, k;
    static string name;
    int type;
    function<string (void)> tostring();
    LabyrinthObject ();
    void remove();
};

class Player : public LabyrinthObject {
public:
    vector<Item> items;
    int life;
    bool alive;
    int itemCount(int itemType);
    int itemCount(Item a);
    void deleteItem(int itemType);
    void deleteItem(Item a);
    Player(string Name, int Ammo, int playerLife);
    void take(Player corpse);
    int ammo();
};

class DSU {
private:
    int const static MAXSIZE = 1000;
public:
    int k;
    pair<int, int> parent[MAXSIZE][MAXSIZE];
    DSU(int w, int h);
    pair<int, int> color(int i, int j);
    void merge(int i1, int j1, int i2, int j2);
};

class LabyrinthData {
public:
    double wallProb;
    vector<Treasure> treasures;
    bool useRandomTreasure;
    int treasureCount;
    double loveToilets; // Если хочет ставить в сартир
    double staticTreasureProb;
    bool canPutTreasureTogether;
};

class Labyrinth {
private:
    int const static MAXSIZE = 1000;
public:
    int w;
    int h;
    vector<LabyrinthObject> cell[MAXSIZE][MAXSIZE];               //<>TODODODO
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

class GameLog : Labyrinth {
    //GameLog previousVersion;//Если ловушка переместила тебя.
    Player player;

    vector<Direction> turn;
    int iStart;
    int jStart;
    int iCur;
    int jCur;

    void addObject(Object a);
    GameLog(int w, int h, int ammo, int life, bool border, int i, int j);
    void addMove(Direction direct);
    void addWall(Direction direct, string wall);
};


#endif // LABYRINTH_H
