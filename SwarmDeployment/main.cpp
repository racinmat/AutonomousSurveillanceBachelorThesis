#include "mainwindow.h"
#include "Configuration.h"
//include <QtWidgets/QApplication>
#include "Core.h"
#include <iostream>
#include <memory>

int runGui(int argc, char *argv[])
{
//	QApplication a(argc, argv);
//	MainWindow w;
//	auto configuration = std::make_shared<App::Configuration>();
//	auto core = std::make_shared<App::Core>(configuration);
//	configuration->setCore(core);
//	core->setLogger(w.getLogger());
//
//	w.setConfiguration(configuration);
//	w.setCore(core);
//	w.show();
//
////	core->run();
//
//	int returnValue = a.exec();
	int returnValue = 0;
	return returnValue;
}

int run(int argc, char *argv[])
{
	auto configuration = make_shared<App::Configuration>();
	auto core = make_shared<App::Core>(configuration);
	configuration->setCore(core);	//toto nemohu zavolat uvnit? konstruktoru
	core->run();

	int returnValue = 0;
	cin.get();
	return returnValue;
}

class A
{
public:
	A()
	{
		
	}
	A(double x, double y) : x(x), y(y)
	{

	}
	double x;
	double y;
};

void testing()
{
	shared_ptr<A> a = make_shared<A>();
	a->x = 2;
	a->y = 3;
	shared_ptr<A> b = a;
	cout << b->x;
	a->x = 4;
	cout << b->x;
}

int main(int argc, char *argv[])
{
	//	int returnValue = run(argc, argv);
//	int returnValue = runGui(argc, argv);
//	return returnValue;
	testing();
	return 0;
}

