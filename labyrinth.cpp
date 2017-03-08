#include "labyrinth.h"
#include <cmath>

#include <string>
#include <vector>
#include <functional>
#include <iostream>


using namespace std;

Labyrinth::Labyrinth(int w, int h) {
    this->w = w;
    this->h = h;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w + 1; j++) {
            verticalWalls[i][j] = "empty";
        }
    }
    for (int i = 0; i < h + 1; i++) {
        for (int j = 0; j < w; j++) {
            horizontWalls[i][j] = "empty";
        }
    }
}
Labyrinth::Labyrinth() {
    cerr << "WTF";
    this->w = 10;
    this->h = 10;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w + 1; j++) {
            verticalWalls[i][j] = "empty";
        }
    }
    for (int i = 0; i < h + 1; i++) {
        for (int j = 0; j < w; j++) {
            horizontWalls[i][j] = "empty";
        }
    }
}

bool Labyrinth::checkPos(int i, int j) {
    return (i >= 0 && i < h && j >= 0 && j < w);
}

//По кордам и направлению дает новые корды.
pair<int, int> Labyrinth::move(int i, int j, Direction direct) {
    string direction = direct.getName();
    if (direction == "up") {
        i = i + 1;
    }
    if (direction == "down") {
        i = i - 1;
    }
    if (direction == "left") {
        j = j - 1;
    }
    if (direction == "right") {
        j = j + 1;
    }
    return make_pair(i, j);
}

string Labyrinth::getWall(int i, int j, Direction direct) {
    string direction = direct.getName();
    if (direction == "up" || direction == "down") {
        if (direction == "up") {
            i++;
        }
        return horizontWalls[i][j];
    } else {
        if (direction == "right") {
            j++;
        }
        return verticalWalls[i][j];
    }
}

void Labyrinth::addWall(int i, int j, Direction direct, string wall) {
    string direction = direct.getName();
    if (direction == "up" || direction == "down") {
        if (direction == "up") {
            i++;
        }
        horizontWalls[i][j] = wall;
    } else {
        if (direction == "right") {
            j++;
        }
        verticalWalls[i][j] = wall;
    }
}

tuple<int, int, int> Labyrinth::findPlayer(string name) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int it = 0;
            for (auto tmp : cell[i][j]) {
                if (tmp->type == LabyrinthObject::TYPE_PLAYER) {
                    auto player = static_cast<Player*>(tmp);
                    if (player->name == name) {
                        return make_tuple(i, j, it);
                    }
                }
                it++;
            }
        }
    }
    return make_tuple(w + 1, h + 1, 1);
}

void Labyrinth::movePlayer(string name, Direction direct) {
    string direction = direct.getName();
    tuple<int, int, int> pos = findPlayer(name);

    auto player = static_cast<Player*>(cell[get<0>(pos)][get<1>(pos)][get<2>(pos)]);
    int position = get<2>(pos);
    cell[get<0>(pos)][get<1>(pos)].erase(cell[get<0>(pos)][get<1>(pos)].begin() + position);
    pair<int, int> tmp  = Labyrinth::move(get<0>(pos), get<1>(pos), direct);
    int i = tmp.first;
    int j = tmp.second;
    player->i = i;
    player->j = j;
    player->k = cell[i][j].size();
    cell[i][j].push_back(player);
}

void Labyrinth::killPlayer(string name) {
    //Debug.Log("Try to kill : " + name);
    tuple<int, int, int> playerPos = findPlayer(name);
    auto player = static_cast<Player*>(cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
    player->alive = false;
}

void Labyrinth::addObject(int i, int j, LabyrinthObject* item) {
    item->i = i;
    item->j = j;
    item->k = cell[i][j].size();
    cell[i][j].push_back(item);
}

void Labyrinth:: makeBorder() {
    for (int r = 0; r < h; r++) {
        verticalWalls[r][0] = "border";
        verticalWalls[r][w] = "border";
    }
    for (int c = 0; c < w; c++) {
        horizontWalls[0][c] = "border";
        horizontWalls[h][c] = "border";
    }
}


void Labyrinth::create() {
    DSU* dsu = new DSU(w, h);


    //Строит рандомный остов.
    while (dsu->k > 1) {
        int i = rand() % h;
        int j = rand() % w;
        int k = rand() % 4;
        //var int i = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, h - 0.5 - float.Epsilon));
//vrode tak //var int j = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, w - 0.5 - float.Epsilon));
        //var k : int = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, 4 - 0.5 - float.Epsilon));
        pair<int, int> newPos = move(i, j, Direction(k));
        if (checkPos(newPos.first, newPos.second)) {
            int lastk = dsu->k;
            dsu->merge(i, j, newPos.first, newPos.second);
            if (lastk > dsu->k) {
                wasWall[i][j][k] = true;
                wasWall[newPos.first][newPos.second][(k + 2) % 4] = true;
            }
        }
    }
    //Ставит  остальные стены
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < 4; k++) {
                if (!wasWall[i][j][k]) {
                    float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                    if (random < data.wallProb) {
                        addWall(i, j, Direction(k), "wall");
                    }
                }
            }
        }
    }

    int pos = 0;

    //Put treasures
    for (int i = 0; i <= data.treasures.size(); i++) {
        Treasure* treasure;
        if (i == 0) {
            treasure = new Treasure(Key());
        }
        else
            if (data.useRandomTreasure) {
                treasure = &data.treasures[rand() % data.treasures.size()];
            } else {
                treasure = &data.treasures[pos++];
            }
        pair<int, int> treasurePos;
        int seed = 0; // How many times you tried to choose
        //Choose pos:
        while (true) {
            treasurePos.first = rand() % h;
            treasurePos.second = rand() % w;
            int wallCount = 0;
            for (int j = 0; j < 4; j++) {
                if (getWall(treasurePos.first, treasurePos.second, Direction(j)) != "empty") {
                    wallCount++;
                }
            }
            bool f = true;
            //check if other treasures here:
            if (!data.canPutTreasureTogether) {
                for (auto obj : cell[treasurePos.first][treasurePos.second]) {
                    if (obj->type == LabyrinthObject::TYPE_TREASURE) {
                        f = false;
                    }
                }
            }
            if (!f) {
                seed++;
                continue;
            }
            double prob = wallCount * data.loveToilets + pow(1.1, seed) - 1 + data.staticTreasureProb;
            float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (random < prob) {
                addObject(treasurePos.first, treasurePos.second, treasure);
                break;
            }
            seed++;
        }
    }

    makeBorder();
}
