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
	drawer = make_shared<Ui::GuiDrawer>(ui->graphicsView, this, this, ui->text);
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
	if (configuration != nullptr)
	{
		configuration->setMapNumber(index);
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

void MainWindow::on_stop_clicked()
{
	configuration->setStop(true);
}

shared_ptr<App::LoggerInterface> MainWindow::getLogger() const
{
	return drawer;	//TODO: zjistit, jestli je opravdu v Core t?íd? jako Logger GuiDrawer.
}

void MainWindow::setCore(shared_ptr<App::Core> core)
{
	this->core = core;
}

void MainWindow::setConfiguration(shared_ptr<App::Configuration> configuration)
{
	this->configuration = configuration;
	this->drawer->setConfiguration(configuration);
}

void MainWindow::updateView()
{
	qApp->processEvents();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	painting = true;

	ui->map->setCurrentIndex(configuration->getMapNumber());
	ui->countUav->setValue(configuration->getUavCount());

	painting = false;

}