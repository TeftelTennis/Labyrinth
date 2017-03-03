#include "serverdata.h"
#include "item.h"
#include "trap.h"
#include <vector>

void ServerData::Start() {
    data.treasures = vector<Treasure>();
    data.treasures.push_back(Treasure(Key()));
    data.treasures.push_back(Treasure(Bullet()));
    data.treasures.push_back(Treasure(Landmine()));
}
