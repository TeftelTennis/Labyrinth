#include<string>
#include <vector>

#ifndef SPLITTER_H
#define SPLITTER_H

using namespace std;

class splitter
{
public:
    static vector<string> split(char c, int k, string s);
};

#endif // SPLITTER_H
