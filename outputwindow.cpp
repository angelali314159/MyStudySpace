#include "outputwindow.h"
#include "ui_outputwindow.h"
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

vector<tuple<string, int, int>> outputSchedule;
vector<pair<string, string>> outputSongs;
string coffeeRec;

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

void outputWindow::getOutput(vector<tuple<string, int, int>> s, vector<pair<string, string>> ss)
{
    outputSchedule = s;
    outputSongs = ss;

}

string outputWindow::getCoffeeRec()
{
    srand(time(0));
    int random = 2 + (rand() % 989);
    std::cout << random;

    //read in csv file
    std::ifstream file("coffeeDatabase.csv");
    if (!file.is_open()) {
    }

    std::string line;
    for(int i = 0; i < random; i++){
        std::getline(file, line);
    }
    return line;
    file.close();
}

void outputWindow::setOutput()
{
    //outputting schedule
    for(auto task : outputSchedule){
        ui->studyOutputBox->addItem(QString::fromStdString("Task: " + get<0>(task)));
    }

    //outputting songs
    for(auto song : outputSongs)
    {
        ui->songListBox->addItem(QString::fromStdString("Song: " + get<0>(song) + "       " + get<1>(song)));
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

