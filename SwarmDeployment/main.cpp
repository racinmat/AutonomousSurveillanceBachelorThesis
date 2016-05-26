#include "Configuration.h"
#include "Core.h"
#include <iostream>
#include <memory>
#include "VCollide\ColDetect.h"
#include <locale>
#include <valarray>
#include <unordered_map>
#include "MapFactory.h"
#include "MapProcessor.h"
#include "Gui.h"					//zakomentovat pro noGui
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include "boost/numeric/ublas/io.hpp"
#include <boost/spirit/home/karma.hpp>

#include "dubins_test.h"
#include "Random.h"
#include "CarLikeAnalyticMotionModel.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#define PI 3.14159265358979323846
#include <filesystem>

using std::cout;
using std::endl;
using namespace boost::numeric;
using namespace boost::spirit::karma;

namespace App
{

	bool fileExists(const std::string& filename) {
		ifstream ifile(filename.c_str());
		return bool(ifile);
	}

	int runResamplingAndDubinsOptimization() {

		auto configuration = make_shared<Configuration>();
		auto core = make_shared<Core>(configuration);
		configuration->setMapNumber(0);
		configuration->setCore(core);	//toto nemohu zavolat uvnit? konstruktoru

		vector<double> frequencies;
		std::string filename;

		//get current directory
		HMODULE hModule = GetModuleHandleW(NULL);
		WCHAR wcharPath[MAX_PATH];
		GetModuleFileNameW(hModule, wcharPath, MAX_PATH);
		wstring ws(wcharPath);
		std::string path(ws.begin(), ws.end());
		std::string exeName = "SwarmDeployment.exe";
		path = path.substr(0, path.size() - exeName.size());
		cout << path << endl;
		std::string configFile = "frequencies.json";
		std::string configPath = path + configFile;

		//load frequencies from file
		if (!fileExists(configPath))
		{
			cout << "file \"frequencies.json\" does not exist." << endl;
			return 1;
		}

		ifstream is(configPath);
		mValue value;
		read(is, value);
		is.close();
		auto fileNameData = value.get_obj().at("path");
		filename = fileNameData.get_str();
		auto frequenciesData = value.get_obj().at("frequencies");
		for (auto frequency : frequenciesData.get_array())
		{
			frequencies.push_back(frequency.get_int());
			//			cout << frequency.get_int() << endl;
		}		//end of loading;

				//		cout << filename << endl;
		core->loadAndOptimizeByDubins(filename, frequencies);

		//cin.get();
		return 0;

	}

	int runRRTPath() {

		auto configuration = make_shared<Configuration>();
		auto core = make_shared<Core>(configuration);
		configuration->setMapNumber(3);
		configuration->setCore(core);	//toto nemohu zavolat uvnitø konstruktoru

		core->runRRTPath();

		//cin.get();
		return 0;

	}

	int run(int argc, char *argv[])
	{
		auto configuration = make_shared<Configuration>();
		auto core = make_shared<Core>(configuration);
		configuration->setMapNumber(0);
		configuration->setCore(core);	//toto nemohu zavolat uvnit? konstruktoru
//		core->run();

//		runResamplingAndDubinsOptimization();
		runRRTPath();
		return 0;
	}

//	class A
//	{
//	public:
//		A()
//		{
//		}
//		A(const A& other) : x(other.x), y(other.y)
//		{
//		}
//		A(double x, double y) : x(x), y(y)
//		{
//		}
//		double x;
//		double y;
//	};
//
//	template <class T>
//	inline void hash_combine(size_t& seed, const T& v)
//	{
//		hash<T> hasher;
//		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//	}


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


//		auto rect1 = Rectangle2D(0, 0, 2, 2);
//		auto rect2 = Rectangle2D(2.1, 2, 2, 2);

//		auto start = clock();

//		cout << ColDetect::coldetect(rect1, rect2) << endl;
			
//		auto duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//		cout << to_string(duration) << " miliseconds to calculate coldetect" << endl;

//		start = clock();

//		cout << ColDetect::coldetectWithoutTransformation(rect1, rect2) << endl;

//		duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//		cout << to_string(duration) << " miliseconds to calculate coldetect after refactoring" << endl;


//		auto triangles = rect1.toTriangles();
//
//		start = clock();
//
//		for (size_t i = 0; i < 50000000; i++)
//		{
//			double v1[3], v2[3], v3[3];
//			for (int j = 0; j < triangles.size(); j++) {
//				auto tri = triangles[j];
//				auto p1 = tri.getPoint1();
//				v1[0] = p1.getX();
//				v1[1] = p1.getY();
//				v1[2] = p1.getZ();
//
//				auto p2 = tri.getPoint2();
//				v2[0] = p2.getX();
//				v2[1] = p2.getY();
//				v2[2] = p2.getZ();
//
//				auto p3 = tri.getPoint3();
//				v3[0] = p3.getX();
//				v3[1] = p3.getY();
//				v3[2] = p3.getZ();
//			}
//		}
//
//		duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//		cout << to_string(duration) << " miliseconds to assert with cache" << endl;
//
//		start = clock();
//
//		for (size_t i = 0; i < 50000000; i++)
//		{
//			double v1[3], v2[3], v3[3];
//			for (int j = 0; j < triangles.size(); j++) {
//				auto tri = triangles[j];
//				v1[0] = tri.getPoint1().getX();
//				v1[1] = tri.getPoint1().getY();
//				v1[2] = tri.getPoint1().getZ();
//
//				v2[0] = tri.getPoint2().getX();
//				v2[1] = tri.getPoint2().getY();
//				v2[2] = tri.getPoint2().getZ();
//
//				v3[0] = tri.getPoint3().getX();
//				v3[1] = tri.getPoint3().getY();
//				v3[2] = tri.getPoint3().getZ();
//			}
//		}
//
//		duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//		cout << to_string(duration) << " miliseconds to assert with some cache" << endl;
//
//		start = clock();
//
//		for (size_t i = 0; i < 50000000; i++)
//		{
//			double v1[3], v2[3], v3[3];
//			for (int j = 0; j < triangles.size(); j++) {
//				v1[0] = triangles[j].getPoint1().getX();
//				v1[1] = triangles[j].getPoint1().getY();
//				v1[2] = triangles[j].getPoint1().getZ();
//
//				v2[0] = triangles[j].getPoint2().getX();
//				v2[1] = triangles[j].getPoint2().getY();
//				v2[2] = triangles[j].getPoint2().getZ();
//
//				v3[0] = triangles[j].getPoint3().getX();
//				v3[1] = triangles[j].getPoint3().getY();
//				v3[2] = triangles[j].getPoint3().getZ();
//			}
//		}
//
//		duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//		cout << to_string(duration) << " miliseconds to assert without cache" << endl;


//			// testování rychlosti motion modelu kvùli optimalizaci
//		auto configuration = make_shared<Configuration>();
//		auto core = make_shared<Core>(configuration);
//		//	MapFactory factory;
//		//	auto map = factory.createMaps(configuration->getUavCount())[0];
//		//	MapProcessor processor(make_shared<LoggerInterface>());
//		//	auto nodes = processor.mapToNodes(map, configuration->getAStarCellSize(), configuration->getWorldWidth(), configuration->getWorldHeight(), configuration->getUavSize(), configuration->getAllowSwarmSplitting());
//		//	valarray<int> a = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
//		//	a -= a % configuration->getAStarCellSize();
//		//	a += (configuration->getAStarCellSize() / 2);
//		//	for(auto b : a)
//		//	{
//		//		cout << b << endl;
//		//	}
//
//			
//			auto near_node = make_shared<App::LinkedState>(configuration->getInputCount());
//			auto uavs = vector<shared_ptr<Uav>>();
//			uavs.push_back(make_shared<App::Uav>(2, 20, 0));
////			uavs.push_back(make_shared<App::Uav>(2, 30, 0));
////			uavs.push_back(make_shared<App::Uav>(2, 40, 0));
////			uavs.push_back(make_shared<App::Uav>(2, 50, 0));
//			near_node->setUavs(uavs);
//
//			int distance_of_new_nodes = configuration->getDistanceOfNewNodes();
//			double max_turn = configuration->getMaxTurn();
//			int input_samples_dist = configuration->getInputSamplesDist();
//			int input_samples_phi = configuration->getInputSamplesPhi();
//			int uavCount = near_node->getUavs().size();
//			int inputCount = configuration->getInputCount();
//			vector<shared_ptr<App::Point>> oneUavInputs = vector<shared_ptr<App::Point>>();
//			shared_ptr<App::State> new_node;
//			App::InputGenerator generator(input_samples_dist, input_samples_phi);
//
//
//			clock_t start;
//			double duration;
//
//
//			//poèet všech možných "kombinací" je variace s opakováním (n-tuple anglicky).
//			//inputs jsou vstupy do modelu
////			for (size_t i = 0; i < 10000; i++)
////			{
////				distance_of_new_nodes = 20 * (i % 10);	//10 rùzných vstupù do generátoru, abych zjistil rychlost s cache
//				auto inputs = generator.generateAllInputs(distance_of_new_nodes, max_turn, near_node->getUavs());	//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV
//				//pokud mám 10 vstupù v cache u samotného NTupleGeneratoru, je to asi 2x pomalejší než bez cache. Takže pro samotný NTupleGenerator cache nepoužívat
//				//místo toho použiju cache pro celé generování vstupù
//				//asi 800 ms na 1000 bìhù a 10 vstupù
//				//s cache asi 300 ms na 1000 bìhù a 10 vstupù
////			}
////			duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
////
////			cout << to_string(duration) << " miliseconds to make inputs 1000 times" << endl;
//
//
//			auto stateFactory = make_shared<StateFactory>(configuration);
//			size_t cycles = 10;
//
//			auto newMotionModel = make_shared<CarLikeAnalyticMotionModel>(configuration, make_shared<LoggerInterface>());
//
//			start = clock();
//
//			for (size_t i = 0; i < inputs.size(); i++)
//			{
//				auto input = inputs[i];
//				auto newNode = stateFactory->createState(*near_node.get());	//copy constructor is called, makes deep copy
//																		// Simulation length
//				double end_time = configuration->getTimeStep();
//
//				//main simulation loop
//				//todo: všude, kde používám push_back se podívat, zda by nešlo na zaèátku naalokovat pole, aby se nemusela dynamicky mìnit velikost
//
//				for (auto uav : newNode->getUavs())
//				{
//					auto uavPointParticle = uav->getPointParticle();
//
//					for (size_t j = 0; j < cycles; j++)
//					{
////						oldMotionModel->calculateState(uavPointParticle, input[*uav.get()]);
//					}
//				}
//			}
//			duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//
//			cout << to_string(duration) << " miliseconds to calculate numeric car like motion model" << endl;
//		
//
//			start = clock();
//			double count = 0;
//
//			for (size_t i = 0; i < 1; i++)
//			{
//				auto input = inputs[i];
//				auto newNode = stateFactory->createState(*near_node.get());	//copy constructor is called, makes deep copy
//																			// Simulation length
//				double end_time = configuration->getTimeStep();
//
//				//main simulation loop
//				//todo: všude, kde používám push_back se podívat, zda by nešlo na zaèátku naalokovat pole, aby se nemusela dynamicky mìnit velikost
//
//				for (auto uav : newNode->getUavs())
//				{
//					auto uavPointParticle = uav->getPointParticle();
//
//					cout << "starting to calculate next states:" << endl;
//					cout << *uavPointParticle << endl;
//					for (size_t j = 0; j < cycles; j++)
//					{
//						newMotionModel->calculateState(uav, input[*uav.get()]);
//						cout << *uavPointParticle << endl;
//					}
//				}
//			}
//
//			duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//
//			cout << to_string(duration) << " miliseconds to calculate analytic car like motion model" << endl;
//
//			cout << "inputs count: " << inputs.size() << endl;
//			cout << "new motion model curve " << newMotionModel->getMinimalCurveRadius() << endl;

		// toto nemodifikuje pole
		//	auto ratios = vector<double>(5); //pomìry jednotlivých ploch ku celkové ploše. Dlouhé jako poèet cílù, tedy poèet guiding paths
		//	double totalVolume = 0;
		//	for (size_t i = 0; i < 5; i++)
		//	{
		//		double volume = i;
		//		ratios[i] = volume;
		//		totalVolume += volume;
		//	}
		//	for_each(ratios.begin(), ratios.end(), [totalVolume](double ratio) { return ratio /= totalVolume; });	//každý prvek je v rozsahu od 0 do 1
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
		//	map1["Tøi"] = 3;
		//
		//	unordered_map<B, int, BHasher> map;
		//	map[*a.get()] = 1;
		//	map[*b.get()] = 2;
		//	map[*c.get()] = 3;
		//	map[*d.get()] = 4;
		//
		//	//heureka, koneènì pro mapu a == b
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
		//	//heureka, koneènì pro mapu a == b
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
		//	copy_if(vec.begin(), vec.end(), vec2.begin(), [](int i) {return i % 2 == 0; });	//po zkopírování jsou ve zbytku nového pole nuly
		//	copy_if(vec.begin(), vec.end(), vec2.begin(), [](int i) {return false; });	//po zkopírování jsou všude nuly
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
		//
		//	MATRIX TESTING AND EXAMPLES
		//
//			ublas::matrix<double> m(3, 3, 4);
//			cout << m << endl;
//		
//			boost::numeric::ublas::vector<double> v(2, 3);
//			cout << v << endl;
//		//
//		////	project(m, ublas::range(1, 1), ublas::range(0, 2)) = v;
//		//
//			auto m1 = ublas::matrix<double>(4, 4, 0);
//			m1(1, 1) = 2;
//			m1(1, 2) = 2;
//			m1(2, 1) = 2;
//			m1(2, 2) = 2;
//		
//		
//			ublas::matrix<double> m2 = ublas::matrix<double>(4, 4, 4);
//			ublas::matrix<double> m7 = ublas::matrix<double>(2, 2, 3);
//			ublas::matrix<double> m6 = ublas::matrix<double>(4, 4, 0);
//		//	project(m6, ublas::range(1, 2), ublas::range(1, 2)) = m7;
//			ublas::subrange(m2, 1, 3, 1, 3) = ublas::matrix<double>(2, 2, 3);	//assigning smaller matrix to subvector
//		
//			cout << "m1: " << endl;
//			cout << format_delimited(columns(m1.size2())[auto_], '\t', m1.data()) << endl;
//		
//			cout << "m7: " << endl;
//			cout << format_delimited(columns(m7.size2())[auto_], '\t', m7.data()) << endl;
//		
//			cout << "m6: " << endl;
//			cout << format_delimited(columns(m6.size2())[auto_], '\t', m6.data()) << endl;
//		
//			cout << "m2: " << endl;
//			cout << format_delimited(columns(m2.size2())[auto_], '\t', m2.data()) << endl;
//		
//			ublas::matrix<double> m3 = prod(m1, m2);
//			ublas::matrix<double> m4 = element_prod(m1, m2);
//
//			cout << "m3: " << endl;
//			cout << format_delimited(columns(m3.size2())[auto_], '\t', m3.data()) << endl;
//
//			cout << "m4: " << endl;
//			cout << format_delimited(columns(m4.size2())[auto_], '\t', m4.data()) << endl;
//
//			m4 = element_prod(m4, m2);
//			cout << "m4: " << endl;
//			cout << format_delimited(columns(m4.size2())[auto_], '\t', m4.data()) << endl;
//
//			m4 = element_prod(m4, m2);
//			cout << "m4: " << endl;
//			cout << format_delimited(columns(m4.size2())[auto_], '\t', m4.data()) << endl;

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
		//	cout << isfinite(p->getX()) << endl;	//vrací false
		//	cout << isfinite(p->getY()) << endl;	//vrací false

		//// práce s poli
		//	vector<int> vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		//	vector<int> vec2 = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
		//	for (auto e : vec1) {
		//		cout << e << ", ";
		//	}	
		//	cout << endl;
		//	for (auto e : vec2) {
		//		cout << e << ", ";
		//	}
		//	cout << endl;
		//
		//	vec1 = vector<int>(vec2.begin() + 3, vec2.end());	//subvector, vykousnutí èásti vektoru
		//	for (auto e : vec1) {
		//		cout << e << ", ";
		//	}
		//	cout << endl;
		//	vec1 = vector<int>(vec2.begin() + 3, vec2.begin() + 7);	//subvector, vykousnutí èásti vektoru
		//	for (auto e : vec1) {
		//		cout << e << ", ";
		//	}
		//	cout << endl;
		//
		//	vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		//	for (auto e : vec1) {
		//		cout << e << ", ";
		//	}
		//	cout << endl;
		//	//	//spojení 3 èástí cesty
		//	vector<int> vec3 = vector<int>(vec1.begin(), vec1.begin() + 4);
		//	vec3.insert(vec3.end(), vec2.begin(), vec2.end());		//vložení za pole
		//	for (auto e : vec3) {
		//		cout << e << ", ";
		//	}
		//	cout << endl;
		//
		//	vec3.insert(vec3.begin() + 2, vec2.begin(), vec2.end());	//vložení dovnitø pole, ne na kraj
		//	for (auto e : vec3) {
		//		cout << e << ", ";
		//	}
		//	cout << endl;

		//
		//
		//	vector<int> path = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		//	vector<int> trajectoryPart = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
		//	vector<int> pathFirstPart;		//èást pøed dubinsem
		//	vector<int> pathMiddlePart = trajectoryPart;	//èást nahrazená dubinsem
		//	vector<int> pathLastPart;		//èást po dubinsovi
		//
		//	int startIndex = 5;
		//	int endIndex = 8;
		//
		//	if (startIndex > 0)	//pøed vyseknutou èástí je další èást
		//	{
		//		pathFirstPart = vector<int>(path.begin(), path.begin() + startIndex);	//subvector, vykousnutí èásti vektoru
		//	}
		//
		//	if (endIndex < path.size() - 1)	//po vyseknuté èásti je ještì další èást
		//	{
		//		pathLastPart = vector<int>(path.begin() + endIndex + 1, path.end());	//subvector, vykousnutí èásti vektoru
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


		//	narovnávání zkøížených cest 1
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
		//	path = pathHandler.straightenCrossingTrajectories(path);
		//		
		//	for (auto state : path)
		//	{
		//		cout << *state << endl;
		//	}

		//	narovnávání zkøížených cest 2
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
		//	state3->getUav(uav2)->getPointParticle()->setLocation(make_shared<Point>(2, 1));
		//	state3->getUav(uav3)->getPointParticle()->setLocation(make_shared<Point>(4, 0.5));
		//	state3->getUav(uav4)->getPointParticle()->setLocation(make_shared<Point>(2, 2));
		//
		//	auto state4 = factory.createState(*state1.get());
		//	state4->getUav(uav1)->getPointParticle()->setLocation(make_shared<Point>(0, 3));
		//	state4->getUav(uav2)->getPointParticle()->setLocation(make_shared<Point>(3, 3));
		//	state4->getUav(uav3)->getPointParticle()->setLocation(make_shared<Point>(4, 3));
		//	state4->getUav(uav4)->getPointParticle()->setLocation(make_shared<Point>(2, 3));
		//
		//	vector<shared_ptr<LinkedState>> linkedPath = { state1, state2, state3, state4 };
		//
		//	auto path = pathHandler.createStatePath(linkedPath);
		//
		//	path = pathHandler.straightenCrossingTrajectories(path);
		//		
		//	for (auto state : path)
		//	{
		//		cout << *state << endl;
		//	}

//		vector<int> path = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//		//	path.erase(path.begin() + 2);
//		//	path.erase(path.begin() + 4);
//		//	path.erase(path.begin() + 5);
//		int i = 0;
//		for (auto state : path)
//		{
//			if (state % 3 == 0)
//			{
//				path.erase(path.begin() + i);
//				i--;
//			}
//			i++;
//		}
//
//		for (auto state : path)
//		{
//			cout << state << endl;
//		}

//		char* buffer = new char[100];
//		
//		// Get the current working directory: 
//		buffer = _getcwd(buffer, 100);
//		printf("%s \nLength: %d\n", buffer, strlen(buffer));
//		free(buffer);
//		
//
//		Persister persister = Persister();
//		auto tuple = persister.loadPathFromJson("C:\\Users\\Azathoth\\Documents\\visual studio 2015\\Projects\\SwarmDeployment\\Win32\\Release\\path-02-29-12-19-16-resampled.json");
//
//		vector<shared_ptr<State>> path;
//		shared_ptr<Map> map;
//		tie(path, map) = tuple;

//		auto start = clock();

//		DistanceResolver resolver = DistanceResolver(make_shared<Configuration>());
//		for (size_t i = 0; i < 100; i++)
//		{
//			resolver.getLengthOfPath(path);
//		}
//		auto duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);
//
//		cout << to_string(duration) << " miliseconds to calculate distance" << endl;


//		double uav_size = 6;
//		auto oldUavLocation = make_shared<Point>(80, 50);
//		auto newUavLocation = make_shared<Point>(94.03206, 54.56);
//		auto distance = oldUavLocation->getDistance(newUavLocation);
//		
//		double xOld = oldUavLocation->getX();
//		double yOld = oldUavLocation->getY();
//		double xNew = newUavLocation->getX();
//		double yNew = newUavLocation->getY();
//		
//		double middleX = (xOld + xNew) / 2;
//		double middleY = (yOld + yNew) / 2;
//
//		Rectangle2D uavRectangle(middleX - distance / 2 - uav_size / 2, middleY - uav_size / 2, distance + uav_size, uav_size);
//
//		cout << "distance:" << distance << endl;
//		cout << "middle:" << middleX << ", " << middleY << endl;
//
//		cout << "p1: " << uavRectangle.p1.getX() << ", " << uavRectangle.p1.getY() << endl;
//		cout << "p2: " << uavRectangle.p2.getX() << ", " << uavRectangle.p2.getY() << endl;
//		cout << "p3: " << uavRectangle.p3.getX() << ", " << uavRectangle.p3.getY() << endl;
//		cout << "p4: " << uavRectangle.p4.getX() << ", " << uavRectangle.p4.getY() << endl;
//
//		cout << "after rotation: " << endl;
//
//		uavRectangle.rotate(atan2(yNew - yOld, xNew - xOld));
//
//		cout << "p1: " << uavRectangle.p1.getX() << ", " << uavRectangle.p1.getY() << endl;
//		cout << "p2: " << uavRectangle.p2.getX() << ", " << uavRectangle.p2.getY() << endl;
//		cout << "p3: " << uavRectangle.p3.getX() << ", " << uavRectangle.p3.getY() << endl;
//		cout << "p4: " << uavRectangle.p4.getX() << ", " << uavRectangle.p4.getY() << endl;

		auto configuration = make_shared<Configuration>();
		auto collisionDetector = make_shared<CollisionDetector>(configuration);

		//		Matrix m1:
		//		1, 1, 1, 0, 0
		//		1, 1, 1, 0, 0
		//		1, 1, 1, 0, 0
		//		0, 0, 0, 1, 1
		//		0, 0, 0, 1, 1

		auto m1 = ublas::matrix<double>(5, 5, 0);

		subrange(m1, 0, 3, 0, 3) = ublas::matrix<double>(3, 3, 1);
		subrange(m1, 3, 5, 3, 5) = ublas::matrix<double>(2, 2, 1);

		//		Matrix m1:
		//		1, 1, 1, 0, 0
		//		1, 1, 1, 0, 0
		//		1, 1, 1, 1, 0
		//		0, 0, 1, 1, 1
		//		0, 0, 0, 1, 1

		auto m2 = ublas::matrix<double>(m1);
		m2(3, 2) = 1;
		m2(2, 3) = 1;


		auto start = clock();

//		cout << "m1, not connected:" << endl;
//		cout << collisionDetector->isGraphConnected(m1) << endl;
//
//		cout << "m2, connected:" << endl;
//		cout << collisionDetector->isGraphConnected(m2) << endl;
		for (size_t i = 0; i < 1000; i++)
		{
			collisionDetector->isGraphConnected(m1);
			collisionDetector->isGraphConnected(m2);
		}

		auto duration = (clock() - start) * 1000 / double(CLOCKS_PER_SEC);

		cout << to_string(duration) << " miliseconds to check connectivity" << endl;

		cin.get();
	}
}

int main(int argc, char *argv[])
{
	int returnValue = 0;
//	returnValue = run(argc, argv);
	returnValue = runGui(argc, argv);
//	testing();
//	returnValue = dubins_test(argc, argv);
	return returnValue;
}
