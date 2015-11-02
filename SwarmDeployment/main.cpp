#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "Core.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	App::Configuration* configuration = new App::Configuration();
	App::Core* core = new App::Core(configuration);
	core->setLogger(w.getLogger());

	w.setConfiguration(configuration);
	w.setCore(core);
	w.show();

	core->run();

	return a.exec();

}
