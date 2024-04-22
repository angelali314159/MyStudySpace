
#include <string>
#include <vector>
#include "taskGraph.h"

using namespace std;

class ScheduleMaker {
  TaskGraph taskGraph;
public:
  vector<tuple<string, int, int>> CreateSchedule(vector<tuple<string, int, vector<string>>> tasks);
  void clear();
};

int getBreak(int val);
string getEndTime(string startTime, int minutes);
