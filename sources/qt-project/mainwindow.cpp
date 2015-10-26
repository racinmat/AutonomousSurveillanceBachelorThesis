#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mockmodel.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new MockModel()),
    painting(false)
{
    ui->setupUi(this);
    ui->countUav->setRange(1, 10);
    ui->graphicsView->setScene(new QGraphicsScene());

    model->setMap(5);
    model->uavCount = 3;

}

MainWindow::~MainWindow()
{
    delete ui->graphicsView->scene();
    delete ui;
    delete model;
}


void MainWindow::on_map_currentIndexChanged(int index)
{
    model->setMap(index);
    qDebug() << "Index in current_index_changed";
    qDebug() << QString("%1").arg(index);
    if(!painting)
    {
        repaint();
    }
}

void MainWindow::on_countUav_valueChanged(int arg1)
{
    model->uavCount = arg1;
    if(!painting)
    {
        repaint();
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    painting = true;

    ui->map->setCurrentIndex(model->getMap());
    ui->countUav->setValue(model->getUavCount());
    ui->graphicsView->scene()->addRect(20, 20, 20, 20, QPen(Qt::green), QBrush(Qt::green));
    ui->graphicsView->scene()->addRect(100, 300, 50, 100, QPen(Qt::gray), QBrush(Qt::gray));
    ui->graphicsView->scene()->addLine(300, 500, 50, 100, QPen(Qt::gray));
    ui->graphicsView->scene()->addLine(300, 500, 100, 50);

    ui->graphicsView->scene()->addLine(500, 500, 550, 550);
    ui->graphicsView->scene()->addLine(500, 550, 550, 500);

    painting = false;
}
