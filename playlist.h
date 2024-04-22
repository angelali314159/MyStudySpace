#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Jingyi Fu, generates the Playlist object including songName and duration of time
struct Playlist{
    string songName;
    int duration;
};

class PlaylistGenerator{
public:
    static vector<vector<Playlist>> createPlaylists(const vector<pair<string, int>>& songs, vector<tuple<string, int, int>> tasks);
};
