// Evelyn Colon, class methods for ScheduleMaker and some utility functions

#include "ScheduleMaker.h"
#include <sstream>

// generates the schedule
void ScheduleMaker::createSchedule(vector<tuple<string, int, vector<string>>> tasks) {
    for (int i = 0; i < tasks.size(); i++) {
        string name = get<0>(tasks[i]);
        int minutes = get<1>(tasks[i]);
        vector<string> dependentTasks = get<2>(tasks[i]);
        taskGraph.insertEdge(name, minutes, dependentTasks);
    }
    vector<string> sorted = taskGraph.topSort();
    for (int i = 0; i < sorted.size(); i++) {
        string name = sorted[i];
        int minutes = taskGraph.getMinutes(name);
        int breakTime = getBreak(minutes);
        tuple<string, int, int> element = make_tuple(name, minutes, breakTime);
        schedule.push_back(element);
    }
}

// returns the schedule vector of tuple
vector<tuple<string, int, int>> ScheduleMaker::getSchedule() {
    return schedule;
}

// returns the length of time a break should be after a task, utility function
int getBreak(int val) {
        if (val < 5) {
            return 1;
        }
        return val / 5;
}

// clears the schedule, class method, unused
void ScheduleMaker::clear() {
    taskGraph.clear();
}

// returns the end time of a task, utility function
string getEndTime(string startTime, int minutes) {
    istringstream stream(startTime);
    string hourToken;
    getline(stream, hourToken, ':');
    string minuteToken;
    getline(stream, minuteToken);
    int hour = stoi(hourToken);
    int minute = stoi(minuteToken);
    int hourInput = minutes / 60;
    int minuteInput = minutes % 60;
    hour += hourInput;
    minute += minuteInput;
    if (minute >= 60) {
        hour += 1;
        minute -= 60;
    }
    if (hour >= 24) {
        hour = hour - 24;
    }
    string time;
    time += to_string(hour);
    time += ':';
    if (minute < 10) {
        time += '0';
    }
    time += to_string(minute);
    return time;
}
