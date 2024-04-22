#pragma once
#include <string>
#include <vector>
#include "taskGraph.h"

using namespace std;

class ScheduleMaker {
  TaskGraph taskGraph;
  vector<tuple<string, int, int>> schedule;
public:
  void createSchedule(vector<tuple<string, int, vector<string>>> tasks);
  void clear();
  vector<tuple<string, int, int>> getSchedule();
};

int getBreak(int val);
string getEndTime(string startTime, int minutes);
