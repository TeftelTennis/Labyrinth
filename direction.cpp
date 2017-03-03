#include "direction.h"

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
