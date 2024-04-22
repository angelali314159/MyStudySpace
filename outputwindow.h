#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QMainWindow>

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
    void getOutput(vector<tuple<string, int, int>> s, vector<pair<string, string>> ss, string t);
    void setOutput();

private slots:
    void on_againButton_clicked();

private:
    Ui::outputWindow *ui;

    string getCoffeeRec();
};

#endif // OUTPUTWINDOW_H
