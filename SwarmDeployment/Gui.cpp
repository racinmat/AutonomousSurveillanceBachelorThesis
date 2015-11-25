#include "Gui.h"
#include "mainwindow.h"					
#include <QtWidgets/QApplication> 		

int runGui(int argc, char *argv[])
{
	int returnValue = 0;
	QApplication a(argc, argv);
	MainWindow w;
	auto configuration = std::make_shared<App::Configuration>();
	auto core = std::make_shared<App::Core>(configuration);
	configuration->setCore(core);
	core->setLogger(w.getLogger());

	w.setConfiguration(configuration);
	w.setCore(core);
	w.show();

	returnValue = a.exec();
	return returnValue;
}
