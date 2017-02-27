#include "labyrinth.h"
#include <string>
#include <vector>
#include <functional>

using namespace std;

#pragma strict
/*
 * типы стен :
 * 		wall
 * 		door
 * 		empty
 *
 * типы мягких мест :
 * 		item
 * 		ammo
 * 		trap
 * 		empty
 *
 * типы итемов :
 * 		bullet // не совсем итем
 * 		key
 *
 * типы ловушек :
 *
 */


// TODO: Return info from item;
// AddObject - Optimize

string Direction::getName() {
    return name[dir];
}


Direction::Direction() {
    dir = UP;
}

Direction::Direction(int a) {
    dir = a;
}

Direction::Direction(string a) {
    int c = 0;
    for (auto i : name) {
        if (i == a) {
            dir = c;
            return;
        }
        c++;
    }
}

void Direction::turnClockwise() {
    dir = (dir + 3) % 4;
}

void Direction::turnCounterClockwise() {
    dir = (dir + 1) % 4;
}

void Direction::reverse() {
    dir = (dir + 2) % 4;
}

LabyrinthObject::LabyrinthObject() {
    type = TYPE_EMPTY;
    name = "empty";
    function<string (void)> tostring = []() { return name; };

}

void LabyrinthObject::remove() {
    type = TYPE_EMPTY;
    name = "empty";
    function<string (void)> tostring = []() { return name; };
}

int Player::itemCount(int itemType) {
    int ans = 0;
    for (int i = 0; i < items.size(); i++) {
        Item tmp = items[i];
        if (tmp.itemType == itemType) {
            ans++;
        }
    }
    return ans;
}

int Player::itemCount(Item a) {
    return itemCount(a.itemType);
}

void Player::deleteItem(int itemType) {
    int ans = 0;
    for (int i = 0; i < items.size(); i++) {
        Item tmp = items[i];
        if (tmp.itemType == itemType) {
            items.RemoveAt(i);
            return;
        }
    }
}

void Player::deleteItem(Item a) {
    deleteItem(a.itemType);
}


Player::Player(string Name, int Ammo, int playerLife) {
    life = playerLife;
    type = TYPE_PLAYER;
    name = Name;
    alive = true;
    items.clear();
    for (int i = 0; i < Ammo; i++) {
        items.push_back(Bullet());
    }
    tostring = []() {
            string tmp = "player: name = " + name + "; life = " +
                life.Tostring() + "; alive = " + alive.Tostring()+ "; items : ";
            for (auto item : items) {
                tmp += '(' + item.tostring() + ' )';
            }
            return tmp + ";";
         };
}

void Player::take(Player corpse) {
    for (auto i : corpse.items) {
        items.push_back(i);
    }
    corpse.items.clear();
}

int Player::ammo() {
    return itemCount(Bullet());
}


DSU::DSU(int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            parent[i, j] = make_pair(i, j);
        }
    }
    k = w * h;
}

pair<int, int> DSU::color(int i, int j) {
    pair<int, int> tmp;
    if (parent[i, j] == make_pair(i, j)) {
        return make_pair(i, j);
    } else {
        tmp = color(parent[i, j].first, parent[i, j].second);
        parent[i, j] = tmp;
        return tmp;
    }
}

void DSU::merge(int i1, int j1, int i2, int j2) {
    pair<int, int> a = color(i1, j1);
    pair<int, int> b = color(i2, j2);
    if (a == b) {
        return;
    }
    k--;
    parent[b.first, b.second] = a;
}



Labyrinth::Labyrinth(int w, int h) {
    this->w = w;
    this->h = h;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w + 1; j++) {
            verticalWalls[i, j] = "empty";
        }
    }
    for (i = 0; i < h + 1; i++) {
        for (j = 0; j < w; j++) {
            horizontWalls[i, j] = "empty";
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
    Player player;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int it = 0;
            for (LabyrinthObject tmp : cell[i][j]) {
                if (tmp.type == LabyrinthObject.TYPE_PLAYER) {
                    player = tmp;
                    if (player.name == name) {
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
    Player player;
    tuple<int, int, int> pos = findPlayer(name);

    player = cell[pos.get(0), pos.get(1)][pos.get(2)];
    cell[pos.get(0), pos.get(1)].RemoveAt(pos.get(2));
    pair<int, int> tmp  = Labyrinth.move(pos.get(0), pos.get(1), direct);
    int i = tmp.first;
    int j = tmp.second;
    cell[i, j].push_back(player);
    player.i = i;
    player.j = j;
    player.k = cell[i][j].size() - 1;
}

void Labyrinth::killPlayer(string name) {
    Debug.Log("Try to kill : " + name);
    tuple<int, int, int> playerPos = findPlayer(name);
    Player player = cell[playerPos.get(0)][playerPos.get(1)][playerPos.get(2)];
    player.alive = false;
}

void Labyrinth::addObject(int i, int j, LabyrinthObject item) {
    item.i = i;
    item.j = j;
    item.k = cell[i][j].size();
    cell[i][j].push_back(item);
}

voidLabyrinth:: makeBorder() {
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
    DSU dsu = DSU(w, h);

    //Строит рандомный остов.
    while (dsu.k > 1) {
        int i = rand() % h;
        int j = rand() % w;
        int k = rand() % 4;
        //var int i = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, h - 0.5 - float.Epsilon));
//vrode tak //var int j = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, w - 0.5 - float.Epsilon));
        //var k : int = Mathf.RoundToInt(Random.Range(-0.5 + float.Epsilon, 4 - 0.5 - float.Epsilon));
        pair<int, int> newPos = move(i, j, Direction(k));
        if (checkPos(newPos.first, newPos.second)) {
            int lastk = dsu.k;
            dsu.merge(i, j, newPos.first, newPos.second);
            if (lastk > dsu.k) {
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
                    if (Random.value < data.wallProb) {  //что за рандом я не всек
                        addWall(i, j, Direction(k), "wall");
                    }
                }
            }
        }
    }

    int pos = 0;

    //Put treasures
    for (int i = 0; i <= data.treasureCount; i++) {
        Treasure treasure;
        if (i == 0)
            treasure = Treasure(Key());
        else
            if (data.useRandomTreasure) {
                treasure = data.treasures[rand() % data.treasures.Count];
            } else {
                treasure = data.treasures[pos++];
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
                for (LabyrinthObject obj : cell[treasurePos.first, treasurePos.second]) {
                    if (obj.type == LabyrinthObject::TYPE_TREASURE) {
                        f = false;
                    }
                }
            }
            if (!f) {
                seed++;
                continue;
            }

            float prob = wallCount * data.loveToilets + Mathf.Pow(1.1, seed) - 1 + data.staticTreasureProb;
            if (Random.value < prob) {  //опять эта странная штука
                addObject(treasurePos.first, treasurePos.second, treasure);
                break;
            }
            seed++;
        }
    }

    makeBorder();
}

void GameLog::addObject(Object a) {  //тут вроде LabyrinthObject надо
    cell[iCur, jCur].push_back(a);
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
