#include "gamelog.h"


void GameLog::addObject(LabyrinthObject a) {
    cell[iCur][jCur].push_back(&a);
}

GameLog::GameLog(string name,int w, int h, int ammo, int life, bool border, int i, int j) : Labyrinth(w, h) {
    player = Player(name, ammo, life);
    iStart = i;
    jStart = j;
    iCur = i;
    jCur = j;
    if (border) {
        makeBorder();
    }
}

void GameLog::addMove(Direction direct) {
    string direction = direct.getName();
    turn.push_back(direction);
    pair<int, int> tmp = Labyrinth::move(iCur, jCur, direct);
    iCur = tmp.first;
    jCur = tmp.second;
}

void GameLog::addWall(Direction direct, string wall) {
    string direction = direct.getName();
    if (direction == "up" || direction == "down") {
        int j = jCur;
        int i = 0;
        if (direction == "down") {
            i = iCur;
        } else {
            i = iCur + 1;
        }
        horizontWalls[i][j] = wall;
    } else {
        int i = iCur;
        int j = 0;
        if (direction == "left") {
            j = jCur;
        } else {
            j = jCur + 1;
        }
        verticalWalls[i][j] = wall;
    }
}
