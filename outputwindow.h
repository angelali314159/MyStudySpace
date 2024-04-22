#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QMainWindow>
#include "ScheduleMaker.h"

using namespace std;

namespace Ui {
class outputWindow;
}

class outputWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit outputWindow(QWidget *parent = nullptr);
    ~outputWindow();
    void getOutput(ScheduleMaker& s, vector<pair<string, string>> ss, string t, string g);
    void setOutput();

private slots:
    void on_againButton_clicked();

private:
    Ui::outputWindow *ui;

    string getCoffeeRec();
};

#endif // OUTPUTWINDOW_H
