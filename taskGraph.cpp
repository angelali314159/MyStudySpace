// Evelyn Colon, taskGraph implementation

// inspired by Project 2 PageRank submission
// inserts an edge into the graph
#include <unordered_set>
#include <queue>
#include <iostream>
#include "taskGraph.h"

void TaskGraph::insertEdge(string name, int minutes, vector<string> dependentTasks) {
    taskTime.emplace(name, minutes);
    adjList.emplace(name, dependentTasks);
    for (int i = 0; i < dependentTasks.size(); i++) {
        inDegrees[dependentTasks[i]]++;
    }
}

// inspired by topSort psuedocode, module 8b lecture slide 115
// sorts the tasks based on dependencies
vector<string> TaskGraph::topSort() {
    queue<string> q;
    unordered_set<string> visited;
    vector<string> sorted;
    for (auto task : adjList) {
        if((inDegrees.find(task.first) == inDegrees.end() || inDegrees[task.first] == 0) && visited.count(task.first) == 0) {
            q.push(task.first);
            visited.insert(task.first);
        }
        while(!q.empty( ) )
        {
            string curr = q.front();
            sorted.push_back(curr);
            for (auto dep : adjList[curr]) {
                inDegrees[dep]--;
                if (inDegrees[dep] == 0) {
                    q.push(dep);
                    visited.insert(dep);
                }
            }
           // cout << curr << endl;
            q.pop();
        }
    }
    return sorted;
}

// returns the length of time of the function
int TaskGraph::getMinutes(string name) {
    return taskTime[name];
}

// clears the graph--unused
void TaskGraph::clear(){
    adjList.clear();
    taskTime.clear();
    inDegrees.clear();
}
