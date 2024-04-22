#include "outputwindow.h"
#include "ui_outputwindow.h"
#include "mainwindow.h"
#include "ScheduleMaker.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

vector<tuple<string, int, int>> outputSchedule;
vector<pair<string, string>> outputSongs;
string coffeeRec;
string startTimeIn;

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

void outputWindow::getOutput(vector<tuple<string, int, int>> s, vector<pair<string, string>> ss, string t)
{
    outputSchedule = s;
    outputSongs = ss;
    startTimeIn = t;
}

string outputWindow::getCoffeeRec()
{
    srand(time(0));
    int random = 2 + (arc4random() % 989);

    //read in csv file
    //cout << std::file_system::current_path() << endl;

    std::ifstream file("/Users/angelali/Testing/coffeeDatabase.csv");
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
    }
/*
    //outputting songs
    for(auto song : outputSongs)
    {
        ui->songListBox->addItem(QString::fromStdString("Song: " + get<0>(song) + "       " + get<1>(song)));
    }
*/
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
