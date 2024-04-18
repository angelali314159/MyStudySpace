#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "json.h"
#include "curlUtils.h"
#include <utility>
using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

// merge sort
void merge(vector<pair<string, int>>& songs, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    vector<pair<string, int>> L(n1), R(n2);
    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = songs[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = songs[middle + 1 + j];
    // Merge the temp arrays back into songs
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].second <= R[j].second) {
            songs[k] = L[i];
            i++;
        } else {
            songs[k] = R[j];
            j++;
        }
        k++;
    }
    // Copy the remaining elements of L[]
    while (i < n1) {
        songs[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R[]
    while (j < n2) {
        songs[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(vector<pair<string, int>>& songs, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(songs, left, middle);
        mergeSort(songs, middle + 1, right);
        merge(songs, left, middle, right);
    }
}

// quick sort
int partition(vector<pair<string, int>>& songs, int low, int high){
    auto pivot = songs[high].second;
    int i = (low -1);

    for (int j = low; j<= high -1; j++){
        // if the song's duration is smaller than or equal to the pivot
        if (songs[j].second <= pivot){
            i++;
            swap(songs[i], songs[j]);
        }
    }
    swap(songs[i+1], songs[high]);
    return (i+1);
}
void quickSort(vector<pair<string, int>>& songs, int low, int high){
    if (low < high){
        int pi = partition(songs, low, high);
        quickSort(songs, low, pi-1);
        quickSort(songs, pi+1, high);
    }
}

// compare merge sorts and quick sorts
void compareSorts(vector<pair<string, int>>& songs){
    vector<pair<string, int>> songsForMergeSort = songs;
    vector<pair<string, int>> songsForQuickSort = songs;

    // quick sort
    auto start = high_resolution_clock::now();
    quickSort(songs, 0, songsForQuickSort.size() - 1);
    auto stop = high_resolution_clock::now();
    auto durationQuickSort = duration_cast<microseconds>(stop - start);

    // merge sort
    start = high_resolution_clock::now();
    mergeSort(songs, 0, songsForMergeSort.size() - 1);
    stop = high_resolution_clock::now();
    auto durationMergeSort = duration_cast<microseconds>(stop - start);

    // Outputting the comparison results
    cout << "Quick Sort took: " << durationQuickSort.count() << " microseconds" << endl;
    cout << "Merge Sort took: " << durationMergeSort.count() << " microseconds" << endl;
}

//return a string of artists' IDs
vector<string> searchArtistsByGenre(const string& genre, const string& token){
    vector<string> artists_ID;
    string query = "genre:%22jazz%22";
    string endpoint = "/v1/search";
    map<string, string> options = {
            {"q", query},
            {"type", "artist"},
            {"limit", "20"} // Adjust
    };

    options["Authorization"] = "Bearer " + token;

    // make the GET request to spotify
    json response;
    try {
        response = SpotifyGET(endpoint, options, token);
    } catch (const exception& e) {
        cerr << "Failed to search artists by genre: " << e.what() << endl;
        return artists_ID; // Return empty list on failure
    }

    // Parse the response and extract artist IDs
    if (response.contains("artists") && response["artists"]["items"].is_array()) {
        for (const auto& item : response["artists"]["items"]) {
            if (item.contains("id") && item["id"].is_string()) {
                artists_ID.push_back(item["id"].get<string>());
            }
        }
    }
    return artists_ID;
}

// return a pair of vectors, which contains string song, and int duration in milliseconds
vector<pair<string, int>> getArtistsTopTracks(const string& artistsID, const string& token){
    vector<pair<string, int>> topTracks;
    // The endpoint to get an artist's top tracks. We assume "market=US";
    string endpoint = "/v1/artists/" + artistsID + "/top-tracks?market=US";
    // make the GET request to Spotify
    json response = SpotifyGET(endpoint, {}, token);
    // parse the response and return the vector pair of tracks
    if (response.contains("tracks") && response["tracks"].is_array()) {
        for (const auto& track : response["tracks"]) {
            string name = track.value("name", "");
            int duration_ms = track.value("duration_ms", 0);
            topTracks.emplace_back(name, duration_ms);
        }
    }
    return topTracks;
};

string refreshToken() {
    string clientId = "d5c3d2aa60854adf85215434d7d9afe3";
    string clientSecret = "f4ec1b86020a48139856ff3e776adcfc";
    string command = "curl -X POST \"https://accounts.spotify.com/api/token\" -H \"Content-Type: application/x-www-form-urlencoded\" -d \"grant_type=client_credentials&client_id=" + clientId + "&client_secret=" + clientSecret + "\"";


    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cerr << "Error executing curl command" << endl;
        return "";
    }
    stringstream buffer;
    char temp[128];
    while (!feof(pipe)) {
        if (fgets(temp, 128, pipe) != NULL)
            buffer << temp;
    }
    pclose(pipe);

    json response = json::parse(buffer.str());
    if (response.contains("access_token") && response["access_token"].is_string()) {
        return response["access_token"].get<string>();
    } else {
        cerr << "Error parsing token from response" << endl;
        return "";
    }
}


int main() {
    string genre = "jazz";
    string token = refreshToken();

    cout << "New token: " << token << endl;

    // 1. search related artists by genre
    vector<string> artistIds = searchArtistsByGenre(genre, token);

    // 2. get top tracks for each artist
    vector<pair<string, int>> songs; // Song name and duration
    for (const auto& artistId : artistIds) {
        auto artistSongs = getArtistsTopTracks(artistId, token);
        for (const auto& artistSong : artistSongs) {
            // Check if the song already exists in the list
            bool found = false;
            for (const auto& song : songs) {
                if (song.first == artistSong.first && song.second == artistSong.second) {
                    found = true;
                    break;
                }
            }
            // If the song is not found in the list, add it
            if (!found) {
                songs.push_back(artistSong);
            }
        }
    }

    // 3. search songs by duration
    compareSorts(songs);
    quickSort(songs, 0, songs.size()-1);
    // vector<pair<string, int>> songs
    cout << "Songs in the genre " << genre << ":" << endl;
    for (const auto& song : songs) {
        cout << "Song Name: " << song.first << ", Duration: " << song.second << "ms" << endl;
    }

    return 0;
}



