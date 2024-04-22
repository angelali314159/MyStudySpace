#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "playlist.h"
using namespace std;

// create playlist using first-fit bin-packing algorithm
    vector<Playlist> PlaylistGenerator::createPlaylist(const vector<pair<std::string, int>> &songs, int task_duration) {
        std::vector<Playlist> playlist;
        int totalDuration = 0;
        int task_duration_seconds = task_duration*60;

    for (const auto& song : songs) {
        // Convert duration from milliseconds to seconds
        int durationSeconds = song.second / 1000;

        // Iterate over sorted songs
    if (totalDuration + durationSeconds > task_duration_seconds) {
        break;
    }

    Playlist playlistItem;
    playlistItem.songName = song.first;
    playlistItem.duration = durationSeconds;
    playlist.push_back(playlistItem);

    totalDuration += durationSeconds;
}
return playlist;
}