//include "mainwindow.h"		//zakomentovat pro noGui
#include "Configuration.h"
//include <QtWidgets/QApplication> 		//zakomentovat pro noGui
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

//	core->run();

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
	A(const A& other) : x(other.x), y(other.y)
	{
	}
	A(double x, double y) : x(x), y(y)
	{
	}
	double x;
	double y;
};
class B
{
public:
	B()
	{
	}
	B(const B& other)
	{
		for (auto a : other.as)
		{
			as.push_back(make_shared<A>(*a.get()));
		}
	}
	vector<shared_ptr<A>> as;
};

void testing()
{
//	shared_ptr<A> a = make_shared<A>();
//	a->x = 2;
//	a->y = 3;
//	shared_ptr<A> d = a;
//	shared_ptr<A> c = make_shared<A>(*a.get());	//using copy constructor
//	cout << "a:" << a->x << endl;	//2
//	cout << "d:" << d->x << endl;	//2
//	cout << "c:" << c->x << endl;	//2
//	a->x = 4;
//	cout << "a:" << a->x << endl;	//4
//	cout << "d:" << d->x << endl;	//4
//	cout << "c:" << c->x << endl;	//2
//
//
//	shared_ptr<B> b = make_shared<B>();
//	b->as.push_back(a);
//	b->as[0]->x = 3;
//	shared_ptr<B> e = make_shared<B>(*b.get());
//	cout << "b->as[0]:" << b->as[0]->x << endl;		//3
//	cout << "e->as[0]:" << e->as[0]->x << endl;		//3
//	b->as[0]->x = 5;
//	cout << "b->as[0]:" << b->as[0]->x << endl;		//5
//	cout << "e->as[0]:" << e->as[0]->x << endl;		//3

	vector<int> arr = {1,2,3,5,8,9};
	int i;
	bool found;
	i= 2;
	found = find(arr.begin(), arr.end(), i) != arr.end();
	cout << found << endl;
	i = 4;
	found = find(arr.begin(), arr.end(), i) != arr.end();
	cout << found << endl;
	cin.get();
}

int main(int argc, char *argv[])
{
	int returnValue = run(argc, argv);
//	int returnValue = runGui(argc, argv);
	return returnValue;
//	testing();
//	return 0;
}

