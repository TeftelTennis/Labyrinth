#include "serverdata.h"
#include "item.h"
#include "trap.h"
#include <vector>
#include <iostream>

using namespace std;

ServerData::ServerData(int width, int height, int startAmmo,
                       int startLife, int keys, int bullets, float wallProb, float staticTreasureProb, float loveToiletsProb,
                       bool canPutTreasureTogether, bool useRandomTreasure) {
    this->width = width;
    this->height = height;
    this->startAmmo = startAmmo;
    this->startLife = startLife;
    data.wallProb = wallProb;
    data.staticTreasureProb = staticTreasureProb;
    data.loveToilets = loveToiletsProb;
    data.canPutTreasureTogether = canPutTreasureTogether;
    data.useRandomTreasure = useRandomTreasure;

    data.treasures = vector<Treasure*>();

    //генерация трежаров (просто запихиваем new тип() в вектор)

    for (int i = 0; i < bullets; i++) {
        data.treasures.push_back(new Treasure (new Bullet()));
    }
    for (int i = 0; i < keys; i++) {
        data.treasures.push_back(new Treasure(new Key()));
    }


    cerr << "serverdataend";
}

ServerData::ServerData() {

}
