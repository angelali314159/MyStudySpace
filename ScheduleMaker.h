#include <string>
#include <vector>
#include "taskGraph.h"

using namespace std;

vector<tuple<string, int, int>> ScheduleMaker(vector<tuple<string, int, vector<string>>> tasks);
int getBreak(int val);
string getEndTime(string startTime, int minutes, bool am);
