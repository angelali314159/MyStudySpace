#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "playlist.h"
using namespace std;

// create playlist using first-fit bin-packing algorithm
    vector<vector<Playlist>> PlaylistGenerator::createPlaylists(const vector<pair<std::string, int>> &songs, vector<tuple<string, int, int>> tasks) {
        vector<vector<Playlist>> playlists(tasks.size());
        vector<int> bins;
        for (int i = 0; i < tasks.size(); i++) {
            bins.push_back(get<1>(tasks[i]));
        }

        for (const auto& song : songs) {
            // Convert duration from milliseconds to seconds
            int durationSeconds = song.second / 1000;
            int gap = MAX_INTEGER;
            int binIndex = -1;
            for (int i = 0; i < tasks.size(); i++) {
                if ((bins[i] - durationSeconds < gap) && (bins[i] - durationSeconds >= 0)) {
                    gap = bins[i] - durationSeconds;
                    binIndex = i;
                }
            }
            if (binIndex == -1) {
                break;
            }
            bins[binIndex] -= durationSeconds;
            Playlist playlistItem;
            playlistItem.songName = song.first;
            playlistItem.duration = durationSeconds;
            playlists[i].push_back(playlistItem);
        }
        return playlists;
    }
