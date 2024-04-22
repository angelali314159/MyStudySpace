
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class TaskGraph {
    unordered_map<string, vector<string>> adjList;
    unordered_map<string, int> taskTime;
    unordered_map<string, int> inDegrees;
public:
    void insertEdge(string name, int minutes, vector<string> dependentTasks);
    vector<string> topSort();
    int getMinutes(string name);
    void clear();
};

