#include "Configuration.h"
#include "Core.h"
#include <iostream>
#include <memory>
#include "VCollide\ColDetect.h"
#include <locale>
#include <valarray>
#include "easyloggingpp-9.80\src\easylogging++.h"
#include <unordered_map>
#include "MapFactory.h"
#include "MapProcessor.h"
//include "Gui.h"					//zakomentovat pro noGui
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include "boost/numeric/ublas/io.hpp"
#include "dubins_test.h"
#include "Random.h"

#define PI 3.14159265358979323846

//INITIALIZE_EASYLOGGINGPP

using std::cout;
using std::endl;
using namespace boost::numeric;

int run(int argc, char *argv[])
{
	auto configuration = make_shared<Configuration>();
	auto core = make_shared<Core>(configuration);
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

template <class T>
inline void hash_combine(size_t& seed, const T& v)
{
	hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


//class B
//{
//
//public:
//	B() : ints(vector<int>()), id(lastId++)
//	{
//	}
//	B(const B& other) : ints(vector<int>())
//	{
//		for (auto a : other.as)
//		{
//			as.push_back(make_shared<A>(*a.get()));
//		}
//		ints = other.ints;
//		id = other.id;
//	}
//
//	friend bool operator==(const B& lhs, const B& rhs)
//	{
//		return lhs.id == rhs.id;
//	}
//
//	friend bool operator!=(const B& lhs, const B& rhs)
//	{
//		return !(lhs == rhs);
//	}
//
//	size_t hash_value() const
//	{
//		size_t seed = 0x2277B8A9;
//		hash_combine(seed, id);
//		return seed;
//	}
//
//	static size_t hash(const B& obj)
//	{
//		return obj.hash_value();
//	}
//
//	vector<shared_ptr<A>> as;
//	vector<int> ints;
//	int id;
//	static int lastId;
//
//};
//
//int B::lastId = 0;
//
//
//class BHasher
//{
//public:
//	size_t operator() (B const& key) const
//	{
//		return key.hash_value();
//	}
//};
//
//class BPtrHasher
//{
//public:
//	size_t operator() (shared_ptr<B> const& key) const
//	{
//		return key->hash_value();
//	}
//};
//
//class EqualFn
//{
//public:
//	bool operator() (B const& t1, B const& t2) const
//	{
//		return t1 == t2;
//	}
//};

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

	
	// testov�n� rychlosti motion modelu kv�li optimalizaci
	auto configuration = make_shared<Configuration>();
	auto core = make_shared<Core>(configuration);
//	MapFactory factory;
//	auto map = factory.createMaps(configuration->getUavCount())[0];
//	MapProcessor processor(make_shared<LoggerInterface>());
//	auto nodes = processor.mapToNodes(map, configuration->getAStarCellSize(), configuration->getWorldWidth(), configuration->getWorldHeight(), configuration->getUavSize(), configuration->getAllowSwarmSplitting());
//	valarray<int> a = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
//	a -= a % configuration->getAStarCellSize();
//	a += (configuration->getAStarCellSize() / 2);
//	for(auto b : a)
//	{
//		cout << b << endl;
//	}

	/*
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
	App::InputGenerator generator(input_samples_dist, input_samples_phi);


	clock_t start;
	double duration;

	start = clock();


	//po�et v�ech mo�n�ch "kombinac�" je variace s opakov�n�m (n-tuple anglicky). 
	//inputs jsou vstupy do modelu
	for (size_t i = 0; i < 10000; i++)
	{
		distance_of_new_nodes = 20 * (i % 10);	//10 r�zn�ch vstup� do gener�toru, abych zjistil rychlost s cache
		auto inputs = generator.generateAllInputs(distance_of_new_nodes, max_turn, near_node->uavs);	//po�et v�ech kombinac� je po�et v�ech mo�n�ch vstup� jednoho UAV ^ po�et UAV
		//pokud m�m 10 vstup� v cache u samotn�ho NTupleGeneratoru, je to asi 2x pomalej�� ne� bez cache. Tak�e pro samotn� NTupleGenerator cache nepou��vat
		//m�sto toho pou�iju cache pro cel� generov�n� vstup�
		//asi 800 ms na 1000 b�h� a 10 vstup�
		//s cache asi 300 ms na 1000 b�h� a 10 vstup�
	}
	duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);

	cout << to_string(duration) << " miliseconds to make inputs 1000 times" << endl;

	/*
	for (size_t i = 0; i < inputs.size(); i++)
	{
		auto input = inputs[i];
		auto tempState = core->car_like_motion_model(near_node, input);	//this method changes near_node
	}

	duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);

	cout << to_string(duration) << " miliseconds to calculate car like motion model" << endl;
*/	

// toto nemodifikuje pole
//	auto ratios = vector<double>(5); //pom�ry jednotliv�ch ploch ku celkov� plo�e. Dlouh� jako po�et c�l�, tedy po�et guiding paths
//	double totalVolume = 0;
//	for (size_t i = 0; i < 5; i++)
//	{
//		double volume = i;
//		ratios[i] = volume;
//		totalVolume += volume;
//	}
//	for_each(ratios.begin(), ratios.end(), [totalVolume](double ratio) { return ratio /= totalVolume; });	//ka�d� prvek je v rozsahu od 0 do 1
//	
//	cout << ratios[0] << endl;		//0
//	cout << ratios[1] << endl;		//1
//	cout << ratios[2] << endl;		//2
//	cout << ratios[3] << endl;		//3
//	cout << ratios[4] << endl;		//4

//	int init[] = { 10,20,30,40 };//     foo:            bar:
//												 
//	valarray<int> foo(init, 4);  //		10 20 30  40
//	valarray<int> bar(25, 4);     //	10 20 30  40    25 25  25  25
//	bar += foo;                     //  10 20 30  40    35 45  55  65
//	foo = bar + 10;                 //  45 55 65  75    35 45  55  65
//	foo -= 10;                      //  35 45 55  65    35 45  55  65
//	valarray<bool> comp = (foo == bar);									//true true true true			 			   	   
//	foo += 35;						//  70 80 90 100    35 45  55  65
//	bar *= 2;						//  70 80 90 100    70 90 110 130
//	valarray<bool> comp1 = (foo == bar);								//true false false false false

//	vector<int> arr1 = { 1, 2, 3, 4, 5 };
//	vector<int> arr2 = { 6, 7, 8, 9, 9 };
//	arr2 = arr1;
//	arr1[0] = 10;
//	cout << arr1[0] << endl;	//10
//	cout << arr2[0] << endl;	//1

//	shared_ptr<B> a = make_shared<B>();
//	shared_ptr<B> c = make_shared<B>();
//	//	a->ints.push_back(5);
//	shared_ptr<B> b = make_shared<B>(*a.get());
//	shared_ptr<B> d = c;
	//	cout << a->ints[0] << endl;
//	cout << b->ints[0] << endl;
//
//	a->ints[0] = 10;
//
//	cout << a->ints[0] << endl;
//	cout << b->ints[0] << endl;

//	unordered_map<string, int> map1;
//	map1["Jedna"] = 1;
//	map1["Dva"] = 2;
//	map1["T�i"] = 3;
//
//	unordered_map<B, int, BHasher> map;
//	map[*a.get()] = 1;
//	map[*b.get()] = 2;
//	map[*c.get()] = 3;
//	map[*d.get()] = 4;
//
//	//heureka, kone�n� pro mapu a == b
//	cout << map[*a.get()] << endl; //2
//	cout << map[*b.get()] << endl; //2
//	cout << map[*c.get()] << endl; //4
//	cout << map[*d.get()] << endl; //4
//
//	cout << "a == b: " << (*a.get() == *b.get()) << endl;	//true
//	cout << "c == d: " << (*c.get() == *d.get()) << endl;	//true
//	cout << "a == c: " << (*a.get() == *c.get()) << endl;	//false
//	cout << "a == d: " << (*a.get() == *d.get()) << endl;	//false
//
//	unordered_map<shared_ptr<B>, int, BPtrHasher> map2;
//	map2[a] = 1;
//	map2[b] = 2;
//	map2[c] = 3;
//	map2[d] = 4;
//
//	//heureka, kone�n� pro mapu a == b
//	cout << map2[a] << endl; //2
//	cout << map2[b] << endl; //2
//	cout << map2[c] << endl; //4
//	cout << map2[d] << endl; //4
//
//	cout << "shared pointers:" << endl;
//	cout << "a == b: " << (*a.get() == *b.get()) << endl;	//true
//	cout << "c == d: " << (*c.get() == *d.get()) << endl;	//true
//	cout << "a == c: " << (*a.get() == *c.get()) << endl;	//false
//	cout << "a == d: " << (*a.get() == *d.get()) << endl;	//false


//	vector<int> vec = {1,2,3,4,5,6,7,8,9};
//	vector<int> vec2 = vector<int>(vec.size());
//	vector<int> vec3 = vector<int>(vec.size());
//	copy_if(vec.begin(), vec.end(), vec2.begin(), [](int i) {return i % 2 == 0; });	//po zkop�rov�n� jsou ve zbytku nov�ho pole nuly
//	copy_if(vec.begin(), vec.end(), vec2.begin(), [](int i) {return false; });	//po zkop�rov�n� jsou v�ude nuly
//
//	vector<int> vec4 = vector<int>(vec.size());
//	vector<int> vec5 = vector<int>(vec.size());
//	copy(vec.begin(), vec.end(), vec4.begin());
//	copy(vec.begin(), vec.end(), vec5.begin());
//
//	for(int i : vec4)
//	{
//		cout << (i % 2 != 0) << endl;
//	}
//	vec4.erase(remove_if(vec4.begin(), vec4.end(), [](int i) {return (i % 2) != 0; }));
//	vec5.erase(remove_if(vec5.begin(), vec5.end(), [](int i) {return true; }));
//
//
//	cout << vec.size() << endl;
//	cout << vec2.size() << endl;
//	cout << vec3.size() << endl;
//
//	ublas::matrix<double> m(3, 3, 4);
//	cout << m << endl;
//
//	boost::numeric::ublas::vector<double> v(2, 3);
//	cout << v << endl;
//
////	project(m, ublas::range(1, 1), ublas::range(0, 2)) = v;
//
//	auto m1 = ublas::matrix<double>(4, 4, 0);
//	m1(1, 1) = 2;
//	m1(1, 2) = 2;
//	m1(2, 1) = 2;
//	m1(2, 2) = 2;
//
//
//	auto m2 = ublas::matrix<double>(4, 4, 4);
//	auto m7 = ublas::matrix<double>(2, 2, 3);
//	auto m6 = ublas::matrix<double>(4, 4, 0);
////	project(m6, ublas::range(1, 2), ublas::range(1, 2)) = m7;
//	ublas::subrange(m2, 1, 3, 1, 3) = ublas::matrix<double>(2, 2, 3);	//assigning smaller matrix to subvector
//
//	for (size_t i = 0; i < m1.size1(); i++)
//	{
//		cout << row(m1, i) << endl;
//	}
//	cout << endl;
//
//	for (size_t i = 0; i < m7.size1(); i++)
//	{
//		cout << row(m7, i) << endl;
//	}
//	cout << endl;
//
//	for (size_t i = 0; i < m6.size1(); i++)
//	{
//		cout << row(m6, i) << endl;
//	}
//	cout << endl;
//
//	for (size_t i = 0; i < m2.size1(); i++)
//	{
//		cout << row(m2, i) << endl;
//	}
//	cout << endl;
//
//	auto m3 = prod(m1, m2);
//	auto m4 = element_prod(m1, m2);
//
//	for (size_t i = 0; i < m3.size1(); i++)
//	{
//		cout << row(m3, i) << endl;
//	}
//	cout << endl;
//	for (size_t i = 0; i < m4.size1(); i++)
//	{
//		cout << row(m4, i) << endl;
//	}
//	cout << endl;
//
//	vector<int> arr = {1, 2, 3, 4, 5};
//	for (size_t i = 0; i < 1000; i++)
//	{
//		cout << Random::element(arr) << endl;
//	}

//	auto motionModel = make_shared<CarLikeMotionModel>(configuration);
//
//	auto state = make_shared<PointParticle>(0, 0, PI / 2);
//	auto control = make_shared<CarLikeControl>(50, PI / 200);
//
//	cout << *state.get() << endl;
//	motionModel->calculateState(state, control);
//	cout << *state.get() << endl;
//	motionModel->calculateState(state, control);
//	cout << *state.get() << endl;
//	motionModel->calculateState(state, control);
//	cout << *state.get() << endl;
//	motionModel->calculateState(state, control);
//	cout << *state.get() << endl;
//	motionModel->calculateState(state, control);
//	cout << *state.get() << endl;
//
//	cout << motionModel->getCurveRadius(control);

//	CollisionDetector c;
//	auto p1 = make_shared<Point>(0, 0);
//	auto p2 = make_shared<Point>(10, 0);
//	auto p3 = make_shared<Point>(0, 10);
//	auto p4 = make_shared<Point>(10, 10);
//	auto p = c.lineLineIntersection(p1, p2, p3, p4);
//
//	cout << *p << endl;
//	cout << isfinite(p->getX()) << endl;	//vrac� false
//	cout << isfinite(p->getY()) << endl;	//vrac� false

//// pr�ce s poli
	vector<int> vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vector<int> vec2 = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
	for (auto e : vec1) {
		cout << e << ", ";
	}	
	cout << endl;
	for (auto e : vec2) {
		cout << e << ", ";
	}
	cout << endl;

	vec1 = vector<int>(vec2.begin() + 3, vec2.end());	//subvector, vykousnut� ��sti vektoru
	for (auto e : vec1) {
		cout << e << ", ";
	}
	cout << endl;
	vec1 = vector<int>(vec2.begin() + 3, vec2.begin() + 7);	//subvector, vykousnut� ��sti vektoru
	for (auto e : vec1) {
		cout << e << ", ";
	}
	cout << endl;

	vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (auto e : vec1) {
		cout << e << ", ";
	}
	cout << endl;
	//	//spojen� 3 ��st� cesty
	vector<int> vec3 = vector<int>(vec1.begin(), vec1.begin() + 4);
	vec3.insert(vec3.end(), vec2.begin(), vec2.end());		//vlo�en� za pole
	for (auto e : vec3) {
		cout << e << ", ";
	}
	cout << endl;

	vec3.insert(vec3.begin() + 2, vec2.begin(), vec2.end());	//vlo�en� dovnit� pole, ne na kraj
	for (auto e : vec3) {
		cout << e << ", ";
	}
	cout << endl;

//
//
//	vector<int> path = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//	vector<int> trajectoryPart = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
//	vector<int> pathFirstPart;		//��st p�ed dubinsem
//	vector<int> pathMiddlePart = trajectoryPart;	//��st nahrazen� dubinsem
//	vector<int> pathLastPart;		//��st po dubinsovi
//
//	int startIndex = 5;
//	int endIndex = 8;
//
//	if (startIndex > 0)	//p�ed vyseknutou ��st� je dal�� ��st
//	{
//		pathFirstPart = vector<int>(path.begin(), path.begin() + startIndex);	//subvector, vykousnut� ��sti vektoru
//	}
//
//	if (endIndex < path.size() - 1)	//po vyseknut� ��sti je je�t� dal�� ��st
//	{
//		pathLastPart = vector<int>(path.begin() + endIndex + 1, path.end());	//subvector, vykousnut� ��sti vektoru
//	}
//
//	auto newPath = pathFirstPart;
//	newPath.insert(newPath.end(), pathMiddlePart.begin(), pathMiddlePart.end());
//	newPath.insert(newPath.end(), pathLastPart.begin(), pathLastPart.end());
//
//	for (auto e : newPath) {
//		cout << e << ", ";
//	}
//	cout << endl;


//	narovn�v�n� zk��en�ch cest

//	StateFactory factory(configuration);
//	PathHandler pathHandler(make_shared<CollisionDetector>(configuration));
//
//	auto uav1 = make_shared<Uav>(make_shared<PointParticle>(0, 0, 0));
//	auto uav2 = make_shared<Uav>(make_shared<PointParticle>(1, 0, 0));
//	auto uav3 = make_shared<Uav>(make_shared<PointParticle>(2, 0, 0));
//	auto uav4 = make_shared<Uav>(make_shared<PointParticle>(3, 0, 0));
//	vector<shared_ptr<Uav>> uavs = {uav1, uav2, uav3, uav4};
//
//	auto state1 = factory.createState();
//	state1->setUavs(uavs);
//
//	auto state2 = factory.createState(*state1.get());
//	state2->getUav(uav1)->getPointParticle()->setLocation(make_shared<Point>(0, 1));
//	state2->getUav(uav2)->getPointParticle()->setLocation(make_shared<Point>(1, 1));
//	state2->getUav(uav3)->getPointParticle()->setLocation(make_shared<Point>(2, 0.5));
//	state2->getUav(uav4)->getPointParticle()->setLocation(make_shared<Point>(3, 1));
//
//	auto state3 = factory.createState(*state1.get());
//	state3->getUav(uav1)->getPointParticle()->setLocation(make_shared<Point>(0, 2));
//	state3->getUav(uav2)->getPointParticle()->setLocation(make_shared<Point>(1, 2));
//	state3->getUav(uav3)->getPointParticle()->setLocation(make_shared<Point>(4, 0.5));
//	state3->getUav(uav4)->getPointParticle()->setLocation(make_shared<Point>(3, 2));
//
//	auto state4 = factory.createState(*state1.get());
//	state4->getUav(uav1)->getPointParticle()->setLocation(make_shared<Point>(0, 3));
//	state4->getUav(uav2)->getPointParticle()->setLocation(make_shared<Point>(1, 3));
//	state4->getUav(uav3)->getPointParticle()->setLocation(make_shared<Point>(4, 3));
//	state4->getUav(uav4)->getPointParticle()->setLocation(make_shared<Point>(3, 3));
//
//	vector<shared_ptr<LinkedState>> linkedPath = { state1, state2, state3, state4 };
//
//	auto path = pathHandler.createStatePath(linkedPath);
//
//	pathHandler.straightenCrossingTrajectories(path);
//		
//	for (auto state : path)
//	{
//		cout << *state << endl;
//	}

	cin.get();
}

int main(int argc, char *argv[])
{
//	LOG(DEBUG) << "start of app, testing log.";
	int returnValue = 0;
//	returnValue = run(argc, argv);
//	returnValue = runGui(argc, argv);
	testing();
//	returnValue = dubins_test(argc, argv);
//	return returnValue;
}
