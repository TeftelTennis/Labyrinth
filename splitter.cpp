#include "splitter.h"
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

vector<string> splitter::split(char c, int k, string s)
{

    /*string segment;
    boost::split(ans, s, boost::is_any_of(c));*/
    int iter = 0;
    vector<string> ans;
    while ((s.size() > 0)) {
        if ((iter >= s.size()) || (s[iter] == c)) {
            if (k == (ans.size() - 1)) {
                iter = s.size();
            }
            ans.push_back(s.substr(0, iter));
            if(iter + 1 < s.size()) {
                s = s.substr(iter + 1, s.size());
            }
            else {
                s = "";
            }
            iter = 0;
        }
        else {
            iter++;
        }
    }
    return ans;

}
