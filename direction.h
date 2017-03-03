#include <string>

#ifndef DIRECTION_H
#define DIRECTION_H

using namespace std;

class Direction {
public:
    int UP = 0;
    int LEFT = 1;
    int DOWN = 2;
    int RIGHT = 3;
    string name[4] = {"up", "left", "down", "right"};

    int dir;
    string getName();
    Direction();
    Direction(int a);
    Direction(string a);
    void turnClockwise();
    void turnCounterClockwise();
    void reverse();
};

#endif // DIRECTION_H
