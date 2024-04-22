#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Playlist{
    string songName;
    int duration;
};

class PlaylistGenerator{
    public:
        static vector<Playlist> createPlaylist(const vector<pair<string, int>>& songs, int task_duration);
};