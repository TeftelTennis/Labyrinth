#include "player.h"


string Player::tostring() {
    string tmp = "player: name = " + name + "; life = " +
        to_string(life) + "; alive = " + to_string(alive) + "; items : ";
    for (auto item : items) {
        tmp += "(" + item.tostring() + " )";
    }
    return tmp + ";";
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
            items.erase(items.begin() + i);
            return;
        }
    }
}

void Player::deleteItem(Item a) {
    deleteItem(a.itemType);
}

Player::Player() {
    type = TYPE_PLAYER;
    alive = true;
    items.clear();
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
}

void Player::take(Player& corpse) {
    for (auto i : corpse.items) {
        items.push_back(i);
    }
    corpse.items.clear();
}

int Player::ammo() {
    return itemCount(Bullet());
}
