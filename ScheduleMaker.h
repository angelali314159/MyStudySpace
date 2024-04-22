
#include <string>
#include <vector>
#include "taskGraph.h"

using namespace std;

class ScheduleMaker {
  TaskGraph taskGraph;
  vector<tuple<string, int, int>> schedule;
public:
  createSchedule(vector<tuple<string, int, vector<string>>> tasks);
  void clear();
};

int getBreak(int val);
string getEndTime(string startTime, int minutes);
