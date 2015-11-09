#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	painting(false)
{
	ui->setupUi(this);
	ui->countUav->setRange(1, 10);
	ui->graphicsView->setScene(new QGraphicsScene());
	drawer = new Ui::GuiDrawer(ui->graphicsView, this);
//	ui->graphicsView->setTransform();
	ui->graphicsView->scale(0.9, -0.9);	//scaling of canvas, when I do not want 1000 to be 1000px, but only 900px. 
	//Negative number in scale is switching direction of axis.
	//Fuck, it also reverses texts.
}

MainWindow::~MainWindow()
{
	delete ui->graphicsView->scene();
	delete ui;
}


void MainWindow::on_map_currentIndexChanged(int index)
{
	if(configuration != nullptr)
	{
		configuration->setMapNumber(index, qDebug());
		qDebug() << "succesfully set the map number!";
	}
	qDebug() << "Index in setting mapNumber:" << QString("%1").arg(index);
	if (!painting)
	{
		qDebug() << "going to repaint!";
		repaint();
	}
}

void MainWindow::on_countUav_valueChanged(int arg1)
{
	if (configuration != nullptr)
	{
		configuration->setUavCount(arg1);
	}
//	qDebug() << "Index in setting uavCount:" << QString("%1").arg(arg1);
	if (!painting)
	{
		repaint();
	}
}

void MainWindow::on_start_clicked()
{
	core->run();
}

App::LoggerInterface* MainWindow::getLogger() const
{
	return drawer;	//TODO: zjistit, jestli je opravdu v Core tøídì jako Logger GuiDrawer.
}

void MainWindow::setCore(std::shared_ptr<App::Core>& core)
{
	this->core = core;
}

void MainWindow::setConfiguration(std::shared_ptr<App::Configuration>& configuration)
{
	this->configuration = configuration;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	painting = true;

	ui->map->setCurrentIndex(configuration->getMapNumber());
	ui->countUav->setValue(configuration->getUavCount());
	//ui->graphicsView->scene()->addRect(20, 20, 20, 20, QPen(Qt::green), QBrush(Qt::green));
	//ui->graphicsView->scene()->addRect(100, 300, 50, 100, QPen(Qt::gray), QBrush(Qt::gray));
	//ui->graphicsView->scene()->addLine(300, 500, 50, 100, QPen(Qt::gray));
	//ui->graphicsView->scene()->addLine(300, 500, 100, 50);

	//ui->graphicsView->scene()->addLine(500, 500, 550, 550);
	//ui->graphicsView->scene()->addLine(500, 550, 550, 500);

	painting = false;

}
