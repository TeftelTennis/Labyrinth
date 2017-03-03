#include "gamelog.h"


void GameLog::addObject(LabyrinthObject a) {  //тут вроде LabyrinthObject надо
    cell[iCur][jCur].push_back(&a);
}

GameLog::GameLog(int w, int h, int ammo, int life, bool border, int i, int j) : Labyrinth(w, h) {
    player = Player("HERO", ammo, life);
    //тут было super(w, h)
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
