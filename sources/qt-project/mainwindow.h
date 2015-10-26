#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mockmodel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *e);
    ~MainWindow();

private slots:
    void on_map_currentIndexChanged(int index);

    void on_countUav_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    MockModel *model;
    bool painting;
};

#endif // MAINWINDOW_H
