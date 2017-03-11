#include "server.h"
#include "splitter.h"
#include <iostream>
#include <QtNetwork>

using namespace std;




Server::Server(ServerData serverData) {
    int w = serverData.width;
    int h = serverData.height;
    startAmmo = serverData.startAmmo;
    startLife = serverData.startLife;
    maxPlayer = serverData.maxPlayers;
    commandLog.clear();

    field = new Labyrinth(w, h);

    field->data = serverData.data;

    field->create();


    turnQueue.clear();
    players.clear();
}

//Вход игроков
void Server::killPlayer(string name) {
    if (turnPlayer == name) {
        turnPlayer = turnQueue.front();
        turnQueue.pop_front();
        return;
    }
    field->killPlayer(name);
    tmpQueue.clear();
    while (!turnQueue.empty()) {
        string nameP = turnQueue.front();
        turnQueue.pop_front();
        if (nameP != name) tmpQueue.push_back(nameP);
    }
    turnQueue = tmpQueue;
}



void Server::addPlayer(int i, int j, string nameP) {
    Player tmp = Player(nameP, startAmmo, startLife);
    field->cell[i][j].push_back(new Player(nameP, startAmmo, startLife));
    //в конец
    turnQueue.push_front(nameP);
}

//Обрабатывает движение игрока
string Server::move(string nameP, string direct) {
    Direction direction = Direction(direct);
    string result;
    string nameNext;
    bool hasKey = false;
    tuple<int, int, int> playerPos = field->findPlayer(nameP);
    Player* player = static_cast<Player*> (field->cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
    for (int it = 0; it < player->items.size(); it++) {
        Item* itt = player->items[it];
        if (itt->itemType == Item::ITEM_TYPE_KEY) {
            hasKey = true;
        }
    }
    string wall = field->getWall(get<0>(playerPos), get<1>(playerPos), direction);

    if (wall == "door") {
        if (hasKey) {
            result = "win";
        } else {
            result = "wall";
            nameNext = nameP;
        }
    } else if (wall == "empty") {
        nameNext = turnQueue.front();
        turnQueue.pop_front();
        turnQueue.push_back(nameNext);
        playerPos = field->findPlayer(nameP);
        cerr << get<0>(playerPos) << " " << get<1>(playerPos) << " " << get<2>(playerPos) << "\n";
        field->movePlayer(nameP, direction);
        playerPos = field->findPlayer(nameP);
        cerr << get<0>(playerPos) << " " << get<1>(playerPos) << " " << get<2>(playerPos) << "\n";
        player = static_cast<Player*>(field->cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
        vector<Trap*> traps;
        Trap* trap;
        vector<Player*> corpses;
        Player* corpse;
        bool treasure = false;

        for (auto obj : field->cell[get<0>(playerPos)][get<1>(playerPos)]) {
            if (obj->type == LabyrinthObject::TYPE_TREASURE) {
                treasure = true;
            } else if (obj->type == LabyrinthObject::TYPE_PLAYER) {
                corpse = static_cast<Player*>(obj);
                if (corpse->alive == false) corpses.push_back(corpse);
            } else if (obj->type == LabyrinthObject::TYPE_TRAP) {
                trap = static_cast<Trap*>(obj);
                traps.push_back(trap);
            }
        }

        result = "move";
        result += " " + to_string(corpses.size());
        for (auto corpse : corpses) {
            result += " " + corpse->name;
            player->take(*corpse);
        }
        result += " " + to_string(traps.size());
        for (auto trap : traps) {
            result += " " + trap->trapType;
            trap->cought(*player, field);
        }

        if (treasure) {
            result += " 1";
        } else {
            result += " 0";
        }
    } else {
        result = "wall";
        nameNext = nameP;
    }
    cerr << result << "\n\n";
    turnPlayer = nameNext;
    return nameP + " move " + direct + " " + result + " " + turnPlayer;
}

string Server::shoot(string nameP, string direct, int item) {
    Direction direction = Direction(direct);
    string result = "";
    string nameNext;
    tuple<int, int, int> playerPos = field->findPlayer(nameP);
    Player* player = static_cast<Player*>(field->cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
    Player* tmpPlayer;
    vector<Player*> corpses;
    vector<Player*> victims;

    Item* bullet = static_cast<Item*>(objectFactory.createItem(item));
    player->deleteItem(Bullet());
    pair<int, int> pos = make_pair(get<0>(playerPos), get<1>(playerPos));

    for (;;) {
        for (auto obj : field->cell[pos.first][pos.second]) {
            if (obj->type == LabyrinthObject::TYPE_PLAYER) {
                tmpPlayer = static_cast<Player*>(obj);
                if ((tmpPlayer->name != nameP) && (tmpPlayer->alive)) {
                    victims.push_back(tmpPlayer);
                }
            }
        }
       if (victims.size()) {
            break;
        }

        if (field->getWall(pos.first, pos.second, direction) != "empty") {
            if (!bullet->hitWall(direction)) {
                break;
            }
        }
        pos = field->move(pos.first, pos.second, direction);
    }

    result += to_string(victims.size());

    for (auto iter = victims.begin(); iter != victims.end(); iter++) {
        Player* tmpPlayer = *iter;
        result += " " + tmpPlayer->name;
        bullet->hitPlayer(*tmpPlayer, field);
        if (tmpPlayer->alive == false) {
            killPlayer(tmpPlayer->name);
        }
    }

    if (get<0>(playerPos) == pos.first && get<1>(playerPos) == pos.second) {
        result += " 1";
        for (auto tmpPlayer : victims) {
            if (tmpPlayer->alive == false) {
                player->take(*tmpPlayer);
            }
        }
    } else {
        result += " 0";
    }

    nameNext = turnQueue.front();
    turnQueue.pop_front();
    turnQueue.push_back(nameNext);
    turnPlayer = nameNext;
    return nameP + " shoot " + direct + " " + to_string(item) + " " + result + " " + nameNext;
}

string Server::dig(string nameP) {
    string result = "";
    string nameNext;
    tuple<int, int, int> playerPos = field->findPlayer(nameP);
    Player* player = static_cast<Player*>(field->cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
    vector<Treasure*> treasures;
    bool findEmpty = false;

    for (auto it : field->cell[get<0>(playerPos)][get<1>(playerPos)]) {
        if (it->type == LabyrinthObject::TYPE_TREASURE) {
            treasures.push_back(static_cast<Treasure*>(it));
        }
    }

    result = to_string(treasures.size());
    Item* item;
    Trap* trap;
    for (auto treasure : treasures) {
         if (treasure->content->type == LabyrinthObject::TYPE_TRAP) {
            result += " 1";
            trap = static_cast<Trap*>(treasure->content);
            trap->cought(*player, field);
            result += ' ' + trap->trapType;
        } else {
            result += " 0";
            item = static_cast<Item*>(treasure->content);
            player->items.push_back(item);
            result += " " + to_string(item->itemType);
        }
        treasure->remove();
    }
    nameNext = turnQueue.front();
    turnQueue.pop_front();
    turnQueue.push_back(nameNext);
    turnPlayer = nameNext;
    return nameP + " dig " + result + " " + nameNext;
}

string Server::doTurn(string turn) {
    vector<string> arr = splitter::split(' ', 4, turn);

    string nameP = arr[0];

    if(nameP == "new") {
        int x = stoi(arr[1]);
        int y = stoi(arr[2]);
        string name = arr[3];
        addPlayer(x, y, name);
        string qwe = "data " + to_string(field->w) + " " +  to_string(field->h) + " "
                + to_string(startAmmo) + " " + to_string(startLife) + " " + turnPlayer;
        cerr << qwe << "\n";
        return qwe;
    }
    commandLog.push_back(nameP + ":>" + turn);

    string type = arr[1];

    if (type == "move") {
        return move(nameP, arr[2]);
    } else if (type == "shoot") {
        return shoot(nameP, arr[2], stoi(arr[3]));
    } else if (type == "dig") {
        return dig(nameP);
    }
}

void Server::startGame() {
    turnPlayer = turnQueue.front();
    turnQueue.pop_front();
}


