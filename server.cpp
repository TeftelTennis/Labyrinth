#include "server.h"
#include "splitter.h"
#include <iostream>
using namespace std;

Server::Server() {
}

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

//Вход выход игроков
void Server::killPlayer(string name) {
    if (turnPlayer == name) {
        turnPlayer = turnQueue.front();
        turnQueue.pop_front();
        //netAdmin.sendWhoNext(turnPlayer);
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

void Server::OnPlayerDisconnected () {
    /*var ip : String = player.externalIP;
    Debug.Log(ip + " - DISCONNECTED");
    killPlayer(players[ip]);
    Network.RemoveRPCs(player, 0);
    Network.DestroyPlayerObjects(player);*/
}

void Server::addPlayer(int i, int j, string nameP) {
    Player tmp = Player(nameP, startAmmo, startLife);
    //Debug.Log("addPlayer " + nameP + ' ' + tmp.toString());
    field->cell[i][j].push_back(new Player(nameP, startAmmo, startLife));
    turnQueue.push_back(nameP);
    //var ip : String = player.externalIP;
    //Debug.Log(ip + ' ' + nameP);
    //if (!players.Contains(ip))
    //    players.Add(ip, nameP);
}

//Обрабатывает движение игрока
void Server::move(string nameP, string direct) {
    Direction direction = Direction(direct);
    string result;
    string nameNext;
    bool hasKey = false;
    tuple<int, int, int> playerPos = field->findPlayer(nameP);
    Player* player = static_cast<Player*> (field->cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
    for (int it = 0; it < player->items.size(); it++) {
        Item itt = player->items[it];
        if (itt.itemType == Item::ITEM_TYPE_KEY) {
            hasKey = true;
        }
    }
    string wall = field->getWall(get<0>(playerPos), get<1>(playerPos), direction);

    if (wall == "door") {
        if (hasKey) {
            // : WIN!!!
            //Debug.Log(nameP + " - WINNER!!!");
        } else {
            result = "wall";
            nameNext = nameP;
        }
    } else if (wall == "empty") {
        if (!turnQueue.empty()) {
            nameNext = turnQueue.front();
            turnQueue.pop_front();
        }
        else {
            nameNext = nameP;
        }
        turnQueue.push_back(nameP);
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
        bool treasure;

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
    //sendResultOfTurn(nameP, "move " + direct, result, nameNext);
}

void Server::shoot(string nameP, string direct, int item) {
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
        //Debug.Log(direction.getName());
        pos = field->move(pos.first, pos.second, direction);
    }

    result += victims.size();
    cerr << " victims = " << victims.size();

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

    if (!turnQueue.empty()) {
        nameNext = turnQueue.front();
        turnQueue.pop_front();
    }
    else {
        nameNext = nameP;
    }
    turnQueue.push_back(nameP);
    turnPlayer = nameNext;
    std::cerr << result << " - shoot\n";
    //netAdmin.sendResultOfTurn(nameP, "shoot " + direct + ' ' + item.ToString(), result, nameNext);
}

void Server::dig(string nameP) {
    cerr << "CELLS\n";
    for (int i = 0; i < field->w; i++) {
        for (int j = 0; j < field->h; j++) {
            cerr << field->cell[i][j].size() << " ";
        }
        cerr << "\n";
    }
    string result = "";
    string nameNext;
    tuple<int, int, int> playerPos = field->findPlayer(nameP);
    Player* player = static_cast<Player*>(field->cell[get<0>(playerPos)][get<1>(playerPos)][get<2>(playerPos)]);
    vector<Treasure*> treasures;
    bool findEmpty = false;

    for (auto it : field->cell[get<0>(playerPos)][get<1>(playerPos)]) {
        cerr << "type: " << it->type <<'\n';
        if (it->type == LabyrinthObject::TYPE_TREASURE) {
            treasures.push_back(static_cast<Treasure*>(it));
        }
    }

    result = to_string(treasures.size());
    Item* item;
    Trap* trap;
    for (auto treasure : treasures) {
        //cerr << treasure->content->type << " - TYPE \n";
        if (treasure->content->type == LabyrinthObject::TYPE_TRAP) {
            result += " 1";
            trap = static_cast<Trap*>(treasure->content);
            trap->cought(*player, field);
            result += ' ' + trap->trapType;
        } else {
            result += " 0";
            item = static_cast<Item*>(treasure->content);
            player->items.push_back(*item);
            result += " " + item->itemType;
        }
        treasure->remove();
    }


    if (!turnQueue.empty()) {
        nameNext = turnQueue.front();
        turnQueue.pop_front();
    }
    else {
        nameNext = nameP;
    }
    turnQueue.push_back(nameP);
    turnPlayer = nameNext;
    cerr<< "dig " << result << "\n";
    //sendResultOfTurn(nameP, "dig", result, nameNext);
}

void Server::doTurn(string turn, string nameP) {
    if (nameP != turnPlayer) {
        commandLog.push_back("Server>'" + nameP + "' try to do turn. turnPlayer = " + turnPlayer);
        return;
    }
    commandLog.push_back(nameP + ":>" + turn);
    //string arr[3] = turn.Split([" "], 3, System.StringSplitOptions.None);
    //SPLIT
    vector<string> arr = splitter::split(' ', 3, turn);

    string type = arr[0];

    if (type == "move") {
        move(nameP, arr[1]);
    } else if (type == "shoot") {
        shoot(nameP, arr[1], stoi(arr[2]));
    } else if (type == "dig") {
        dig(nameP);
    }
}

void Server::startGame() {
    turnPlayer = turnQueue.front();
    turnQueue.pop_front();
    //netAdmin.sendWhoNext(turnPlayer);
}


void Server::doCommand(string command) {
    vector<string> com = splitter::split(' ', 100, command);

    tuple<int, int, int> pos;
    Player* player ;
    pair<int, int> tmpos;
    if (com[0] == "kill") {
        killPlayer(com[1]);
    } else if (com[0] == "give") {
        pos = field->findPlayer(com[1]);
        player = static_cast<Player*>(field->cell[get<0>(pos)][get<1>(pos)][get<2>(pos)]);
        player->items.push_back(*static_cast<Item*>(objectFactory.createItem(stoi(com[2]))));
        //netAdmin.sendResultOfTurn(com[1], "dig", "1 item " + com[2], "Server");
    } else if (com[0] == "stat") {
        pos = field->findPlayer(com[1]);
        player = static_cast<Player*>(field->cell[get<0>(pos)][get<1>(pos)][get<2>(pos)]);
        commandLog.push_back(player->tostring());
        commandLog.push_back("           position = " + to_string(get<0>(pos)) + " " + to_string(get<1>(pos)));
    } else if (com[0] == "set_turn") {
        turnQueue.push_back(turnPlayer);
        turnPlayer = com[1];
        //netAdmin.sendWhoNext(com[1]);
    } else if (com[0] == "add") {
        tmpos = make_pair(stoi(com[1]), stoi(com[2]));
        if (com[3] == "trap") {
            int ololo = stoi(com[4]);
            Trap* tt = static_cast<Trap*>(objectFactory.createTrap(ololo));
            field->addObject(tmpos.first, tmpos.second, tt);
        } else if (com[3] == "treasure"){
            if (com[4] == "trap") {
                field->addObject(tmpos.first, tmpos.second, new Treasure(*static_cast<Trap*>(objectFactory.createTrap(stoi(com[5])))));
            } else if (com[4] == "item") {
                field->addObject(tmpos.first, tmpos.second, new Treasure(*static_cast<Item*>(objectFactory.createItem(stoi(com[5])))));
            }
        }
    } else if (com[0] == "get") {
        tmpos = make_pair(stoi(com[1]), stoi(com[2]));
        for (auto obj : field->cell[tmpos.first][tmpos.second]) {
            commandLog.push_back(obj->tostring());
        }
    } else if (com[0] == "get_wall") {
        tmpos = make_pair(stoi(com[1]), stoi(com[2]));
        commandLog.push_back(field->getWall(tmpos.first, tmpos.second, Direction(com[3])));
    } else if (com[0] == "add_wall") {
        tmpos = make_pair(stoi(com[1]), stoi(com[2]));
        field->addWall(tmpos.first, tmpos.second, Direction(com[3]), com[4]);
    } else if (com[0] == "do") {
        for (int i  = 2; i < com.size(); i++) {
            com[1] += " " + com[i];
        }
        doTurn(com[1], turnPlayer);
    }
}


/*function OnGUI() {
    if (isConsoleOpen) {
        GUI.Window(1488, new Rect(0, 0, Screen.width, Screen.height / 3), console, "Server Console", consoleStyle);
    }
    if (Event.current.type == EventType.KeyUp && Event.current.keyCode == KeyCode.BackQuote) {
        isConsoleOpen = !isConsoleOpen;
    }
}

function console(windowID : int) {
    scrollPosition = GUILayout.BeginScrollView (scrollPosition, GUILayout.MaxHeight(Screen.height / 3 - 20));
        var text : String = "";
        for (var com : String in commandLog)
            text += com + '\n';
        GUILayout.Label(text);
    GUILayout.EndScrollView();

    GUI.SetNextControlName ("cmd");
    command = GUI.TextField(new Rect(0, Screen.height / 3 - 20, Screen.width, 20), command);
    GUI.FocusControl ("cmd");
    if (Event.current.isKey && Event.current.keyCode == KeyCode.Return &&
            GUI.GetNameOfFocusedControl () == "cmd" && command != "") {
        commandLog.Add(">>>" + command);
        scrollPosition.y = 10000000;
        doCommand(command);
        command = "";
    }
}*/



