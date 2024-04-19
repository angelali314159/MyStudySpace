#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "ScheduleMaker.h"
using namespace std;

int main() {
    vector<string> dependentTasks1 = {"Complete Review Sheet", "Complete Practice Exam", "Review MATLAB"};
    tuple<string, int, vector<string>> task1 = make_tuple("Watch Review Lectures", 50, dependentTasks1);
    vector<string> dependentTasks2 = {"Review Lecture Notes"};
    tuple<string, int, vector<string>> task2 = make_tuple("Complete Review Sheet", 120, dependentTasks2);
    tuple<string, int, vector<string>> task3 = make_tuple("Complete Practice Exam", 120, dependentTasks2);
    tuple<string, int, vector<string>> task4 = make_tuple("Review MATLAB", 30, dependentTasks2);
    vector<string> emptyDependency;
    tuple<string, int, vector<string>> task5 = make_tuple("Review Lecture Notes", 60, emptyDependency);
    vector<tuple<string, int, vector<string>>> tasks = {task1, task2, task3, task4, task5};
    vector<tuple<string, int, int>> schedule = ScheduleMaker(tasks);
    string prevEndTime = "9:00AM";
    for (int i = 0; i < schedule.size(); i++) {
        string name = get<0>(schedule[i]);
        int minutes = get<1>(schedule[i]);
        int breakTime = get<2>(schedule[i]);
        string startTime = prevEndTime;
        bool morning;
        if (startTime[startTime.length() - 2] == 'A') {
            morning = true;
        }
        else {
            morning = false;
        }
        prevEndTime = getEndTime(startTime, minutes, morning);
        cout << startTime << ": " << name << endl;
        startTime = prevEndTime;
        if (startTime[startTime.length() - 2] == 'A') {
            morning = true;
        }
        else {
            morning = false;
        }
        prevEndTime = getEndTime(startTime, breakTime, morning);
        cout << startTime << ": " << "Break!" << endl;
    }
    return 0;
}
