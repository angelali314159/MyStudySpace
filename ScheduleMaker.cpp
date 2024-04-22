//
// Created by evely on 4/19/2024.
//

#include "ScheduleMaker.h"
#include <sstream>

vector<tuple<string, int, int>> ScheduleMaker(vector<tuple<string, int, vector<string>>> tasks) {
    vector<tuple<string, int, int>> schedule;
    TaskGraph graph;
    for (int i = 0; i < tasks.size(); i++) {
        string name = get<0>(tasks[i]);
        int minutes = get<1>(tasks[i]);
        vector<string> dependentTasks = get<2>(tasks[i]);
        graph.insertEdge(name, minutes, dependentTasks);
    }
    vector<string> sorted = graph.topSort();
    for (int i = 0; i < sorted.size(); i++) {
        string name = sorted[i];
        int minutes = graph.getMinutes(name);
        int breakTime = getBreak(minutes);
        tuple<string, int, int> element = make_tuple(name, minutes, breakTime);
        schedule.push_back(element);
    }
    return schedule;
}

int getBreak(int val) {
        if (val < 5) {
            return 1;
        }
        return val / 5;
}

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
