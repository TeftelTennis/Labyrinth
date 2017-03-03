#include "splitter.h"
#include <vector>
#include <string>

using namespace std;

vector<string> splitter::split(char c, int k, string s)
{
    int iter = 0;
    vector<string> ans;
    while ((s.size() > 0)) {
        if ((iter >= s.size()) || (s[iter] == c)) {
            if (k == (ans.size() - 1)) {
                iter = s.size();
            }
            ans.push_back(s.substr(0, iter));
            s = s.substr(iter, s.size());
            iter = 0;
        }
        else {
            iter++;
        }
    }
    return ans;

}
