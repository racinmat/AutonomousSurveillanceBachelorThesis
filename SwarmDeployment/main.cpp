//include "mainwindow.h"					//zakomentovat pro noGui
#include "Configuration.h"
//include <QtWidgets/QApplication> 		//zakomentovat pro noGui
#include "Core.h"
#include <iostream>
#include <memory>
#include "VCollide\ColDetect.h"
#include <locale>

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

//	vector<int> arr = {1,2,3,5,8,9};
//	int i;
//	bool found;
//	i= 2;
//	found = find(arr.begin(), arr.end(), i) != arr.end();
//	cout << found << endl;		//true
//	i = 4;
//	found = find(arr.begin(), arr.end(), i) != arr.end();
//	cout << found << endl;		//false


//	auto tri_uav = Triangle3D(
//		Point3D(72.811994693764177, 78.646404722814680, 1), 
//		Point3D(73.311994693764177, 78.646404722814680, 1), 
//		Point3D(73.061994693764177, 79.146404722814680, 1));
//	double trans[] = { 3.1284838758924138, 14.529960666841021, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
//	double zero_trans[] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
//
//	auto tri1_obs = vector<Triangle3D>(1);
//	auto tri2_obs = vector<Triangle3D>(1);
//	int j = 0;
//	tri1_obs[0] = Triangle3D(
//		Point3D(0, 400, 1),
//		Point3D(700, 400, 1),
//		Point3D(700, 450, 1));
//	tri2_obs[0] = Triangle3D(
//		Point3D(0, 400, 1),
//		Point3D(0, 50, 1),
//		Point3D(700, 450, 1));
//
//	cout << "tri1:" << ColDetect::coldetect(tri_uav, tri1_obs[j], trans, zero_trans) << endl;
//	cout << "tri2:" << ColDetect::coldetect(tri_uav, tri2_obs[j], trans, zero_trans) << endl;


	auto configuration = make_shared<App::Configuration>();
	auto core = make_shared<App::Core>(configuration);


	auto near_node = make_shared<App::State>(configuration->getInputCount());
	near_node->uavs.push_back(make_shared<App::Uav>(2, 20, 0));
	near_node->uavs.push_back(make_shared<App::Uav>(2, 30, 0));
	near_node->uavs.push_back(make_shared<App::Uav>(2, 40, 0));
	near_node->uavs.push_back(make_shared<App::Uav>(2, 50, 0));

	int distance_of_new_nodes = configuration->getDistanceOfNewNodes();
	double max_turn = configuration->getMaxTurn();
	int input_samples_dist = configuration->getInputSamplesDist();
	int input_samples_phi = configuration->getInputSamplesPhi();
	int uavCount = near_node->uavs.size();
	int inputCount = configuration->getInputCount();
	vector<shared_ptr<App::Point>> oneUavInputs = vector<shared_ptr<App::Point>>();
	shared_ptr<App::State> new_node;

	for (size_t k = 0; k < input_samples_dist; k++)
	{
		for (size_t m = 0; m < input_samples_phi; m++)
		{
			double x = distance_of_new_nodes / pow(1.5, k);
			double y = -max_turn + 2 * m * max_turn / (input_samples_phi - 1);
			shared_ptr<App::Point> point = make_shared<App::Point>(x, y);
			oneUavInputs.push_back(point);
		}
	}

	clock_t start;
	double duration;

	start = clock();




	//poèet všech možných "kombinací" je variace s opakováním (n-tuple anglicky). 
	//inputs jsou vstupy do modelu
	vector<vector<shared_ptr<App::Point>>> inputs = core->generateNTuplet<shared_ptr<App::Point>>(oneUavInputs, uavCount);	//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV

	for (size_t i = 0; i < inputs.size(); i++)
	{
		auto input = inputs[i];
		auto tempState = core->car_like_motion_model(near_node, input);	//this method changes near_node
	}

	duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);

	cout << to_string(duration) << " miliseconds to calculate car like motion model" << endl;


	cin.get();
}

int main(int argc, char *argv[])
{
	int returnValue = 0;
//	returnValue = run(argc, argv);
//	returnValue = runGui(argc, argv);
	testing();
	return returnValue;
}

