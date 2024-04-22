#include "outputwindow.h"
#include "ui_outputwindow.h"
#include "mainwindow.h"
#include "ScheduleMaker.h"
#include "json.h"
#include "curlUtils.h"
#include "playlist.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;

using namespace std;

vector<tuple<string, int, int>> outputSchedule;
vector<vector<Playlist>> outputSongs;
string coffeeRec;
string startTimeIn;
string genre;

//*************************************************************************************************

string formatDuration(int durationSeconds) {
    int minutes = durationSeconds / 60;
    int seconds = durationSeconds % 60;
    return to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);
}

// Jingyi Fu implemented the merge sort and quick sort algorithms, and compared these two sorts according to time difference. 

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
        if (L[i].second >= R[j].second) {
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
        if (songs[j].second >= pivot){
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

// compare merge sorts and quick sorts in milleseconds
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

// return a string of artists' IDs by an inputted genre
vector<string> searchArtistsByGenre(const string& genre, const string& token){
    vector<string> artists_ID;

    /*
        code adapted from the Spotify API for Developers
        https://developer.spotify.com/documentation/web-api/tutorials/code-flow
    */
    
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

/*
    clientId = "d5c3d2aa60854adf85215434d7d9afe3"
    clientSecret = "f4ec1b86020a48139856ff3e776adcfc"
    The above data are from the Spotify for Developers. The ID and the Secret are both unique, and only applicable for this project.
*/

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
//*************************************************************************************************
outputWindow::outputWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::outputWindow)
{
    ui->setupUi(this);
}

outputWindow::~outputWindow()
{
    delete ui;
}

void outputWindow::getOutput(ScheduleMaker& s, string t, string g)
{
    outputSchedule = s.getSchedule();
    startTimeIn = t;
    genre = g;
}

string outputWindow::getCoffeeRec()
{

    srand(time(0));
    int random = 2 + (arc4random() % 989);

    //read in csv file
    //cout << std::file_system::current_path() << endl;

    std::ifstream file("/Users/angelali/Testing/coffeeDatabase.csv"); //CHANGE FILE LOCATION
    if (!file.is_open()) {
        cout << "file didn't open" << endl;
    }

    std::string line;
    for(int i = 0; i < random; i++){
        std::getline(file, line);
    }
    string country = line.substr(0, line.find_first_of(','));
    string rest = line.substr(line.find_first_of(',') + 1);
    string region = rest.substr(0, rest.find_first_of(','));
    string returnString = "From " + region.substr(1, region.size() - 2) + " in " + country.substr(1, country.size() - 2);
    return returnString;
    file.close();
}

void outputWindow::setOutput()
{
    string token = refreshToken();
    vector<string> artistIds = searchArtistsByGenre(genre, token);
    //  get top tracks for each artist
    vector<pair<string, int>> songs; // Song name and duration
    for (const auto& artistId : artistIds) {
        auto artistSongs = getArtistsTopTracks(artistId, token);
        for (const auto& artistSong : artistSongs) {
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
    compareSorts(songs);
    quickSort(songs, 0, songs.size()-1);

    outputSongs = PlaylistGenerator::createPlaylists(songs, outputSchedule);

    string prevEndTime = startTimeIn;
    for (int i = 0; i < outputSchedule.size(); i++) {
        string name = get<0>(outputSchedule[i]);
        int minutes = get<1>(outputSchedule[i]);
        int breakTime = get<2>(outputSchedule[i]);
        string startTime = prevEndTime;
        prevEndTime = getEndTime(startTime, minutes);
        ui->studyOutputBox->addItem(QString::fromStdString(startTime + ": " + name));
        startTime = prevEndTime;
        prevEndTime = getEndTime(startTime, breakTime);
        ui->studyOutputBox->addItem(QString::fromStdString(startTime + ": Break!"));

        ui->songListBox->addItem(QString::fromStdString("*******************************************"));
        ui->songListBox->addItem(QString::fromStdString(get<0>(outputSchedule[i]) + " for " + to_string(get<1>(outputSchedule[i])) + " minutes"));

        for (int x = 0; x < outputSongs[i].size(); x++){
            ui->songListBox->addItem(QString::fromStdString(outputSongs[i][x].songName + "\t" + formatDuration((outputSongs[i][x].duration))));
        }
    }

    //output coffee rec
    coffeeRec = getCoffeeRec();
    ui->coffeeRecLabel->setText(QString::fromStdString(coffeeRec));
}

void outputWindow::on_againButton_clicked()
{
    this->hide();
    outputSchedule.clear();
    outputSongs.clear();
    MainWindow *w = new MainWindow();
    w->show();
}
