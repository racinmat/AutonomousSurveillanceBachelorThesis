#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "Core.h"
#include <iostream>
#include <memory>

int run(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	auto configuration = std::make_shared<App::Configuration>();
	auto core = std::make_shared<App::Core>(configuration.get());
	core->setLogger(w.getLogger());

	w.setConfiguration(configuration);
	w.setCore(core);
	w.show();

	core->run();

	int returnValue = a.exec();

	return returnValue;
}

int main(int argc, char *argv[])
{
	int returnValue = run(argc, argv);
	return returnValue;
}

