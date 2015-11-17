#include "mainwindow.h"					//zakomentovat pro noGui
#include "Configuration.h"
#include <QtWidgets/QApplication> 		//zakomentovat pro noGui
#include "Core.h"
#include <iostream>
#include <memory>
#include "VCollide\ColDetect.h"

int runGui(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	auto configuration = std::make_shared<App::Configuration>();
	auto core = std::make_shared<App::Core>(configuration);
	configuration->setCore(core);
	core->setLogger(w.getLogger());

	w.setConfiguration(configuration);
	w.setCore(core);
	w.show();

	int returnValue = a.exec();
//	int returnValue = 0;
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

//	vector<int> arr = {1,2,3,5,8,9};
//	int i;
//	bool found;
//	i= 2;
//	found = find(arr.begin(), arr.end(), i) != arr.end();
//	cout << found << endl;
//	i = 4;
//	found = find(arr.begin(), arr.end(), i) != arr.end();
//	cout << found << endl;


	auto tri_uav = Triangle3D(
		Point3D(72.811994693764177, 78.646404722814680, 1), 
		Point3D(73.311994693764177, 78.646404722814680, 1), 
		Point3D(73.061994693764177, 79.146404722814680, 1));
	double trans[] = { 3.1284838758924138, 14.529960666841021, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	double zero_trans[] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };

	auto tri1_obs = vector<Triangle3D>(1);
	auto tri2_obs = vector<Triangle3D>(1);
	int j = 0;
	tri1_obs[0] = Triangle3D(
		Point3D(0, 400, 1),
		Point3D(700, 400, 1),
		Point3D(700, 450, 1));
	tri2_obs[0] = Triangle3D(
		Point3D(0, 400, 1),
		Point3D(0, 50, 1),
		Point3D(700, 450, 1));

	cout << "tri1:" << ColDetect::coldetect(tri_uav, tri1_obs[j], trans, zero_trans) << endl;
	cout << "tri2:" << ColDetect::coldetect(tri_uav, tri2_obs[j], trans, zero_trans) << endl;
	cin.get();
}

int main(int argc, char *argv[])
{
	int returnValue = 0;
//	returnValue = run(argc, argv);
	returnValue = runGui(argc, argv);
//	testing();
	return returnValue;
}

