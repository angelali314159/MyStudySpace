#include "./ui_mainwindow.h"
#include "outputwindow.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <vector>
#include <iostream>

using namespace std;

vector<string> dependentTasks;
string taskName;
int minutes = 0;
vector<tuple<string, int, vector<string>>> inputtedTasks;
tuple<string, int, vector<string>> tempTask;
string startTime;
string musicGenre;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_minutesBox_textChanged(const QString &arg1)
{
    string input = arg1.toStdString();
    //only allows digits to be inputted
    if (!isdigit(input[input.size() - 1])){
        ui->minutesBox->backspace();
    }
}


void MainWindow::on_depButtonBox_clicked()
{
    if (ui->depTasksOptionsBox->currentIndex() > 0){
        dependentTasks.push_back(ui->depTasksOptionsBox->currentText().toStdString());
        ui->depTasksOptionsBox->setCurrentIndex(0);
    }
}

void MainWindow::on_finalTaskButton_clicked()
{
    minutes = ui->minutesBox->text().toInt();
    if (minutes != 0){
        taskName = ui->taskNameBox->text().toStdString();
        ui->depTasksOptionsBox->addItem(QString::fromStdString(taskName));
        if (ui->depTasksOptionsBox->currentIndex() > 0){
            dependentTasks.push_back(ui->depTasksOptionsBox->currentText().toStdString());
        }
        string taskString;
        if (dependentTasks.size() != 0){
            taskString = taskName + " for " + to_string(minutes) + " minutes and must be done after ";
            taskString += dependentTasks.at(0);
            for (int i = 1; i < dependentTasks.size(); i++){
                if (i == dependentTasks.size() - 1){
                    taskString += " and ";
                }else{
                    taskString += ", ";
                }
                taskString += dependentTasks.at(i);
            }
        }
        else{
            taskString = taskName + " for " + to_string(minutes);
        }
        tempTask = make_tuple(taskName, minutes, dependentTasks);
        inputtedTasks.push_back(tempTask);
        dependentTasks.clear();
        ui->taskNameBox->clear();
        ui->minutesBox->clear();
        ui->toDoListBox->addItem(QString::fromStdString(taskString));
    }
}

void MainWindow::on_generateButton_clicked()
{
    startTime = ui->startTimeBox->time().toString(QString::fromStdString("H:mm a")).toStdString();
    musicGenre = ui->musicGenreBox->currentText().toStdString();
    this->hide();

    outputWindow *oWindow = new outputWindow();
    //CALL MAKING SCHEDULE FUNCTIONS
    //vector<tuple<string, int, int>> madeSchedule = ScheduleMaker(inputtedTasks);


    //CALL MUSIC PLAYLIST FUNCTIONS
    //vector<pair<string, string>> madeSongs =
    //oWindow->getOutput(madeSchedule, madeSongs);
    oWindow->show();

}

