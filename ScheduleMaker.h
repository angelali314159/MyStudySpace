#pragma once
#include <string>
#include <vector>
#include "taskGraph.h"

using namespace std;
// Evelyn Colon, ScheduleMaker class declaration, along with some utility function declarations

class ScheduleMaker {
  TaskGraph taskGraph; // taskGraph object, see taskGraph class
  vector<tuple<string, int, int>> schedule; // vector of tuples representing a schedule, with each tuple (taskname, tasklength, breaklength)
public:
  void createSchedule(vector<tuple<string, int, vector<string>>> tasks);
  void clear();
  vector<tuple<string, int, int>> getSchedule();
};

int getBreak(int val);
string getEndTime(string startTime, int minutes);
