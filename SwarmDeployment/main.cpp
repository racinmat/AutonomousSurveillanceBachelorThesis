#include "Configuration.h"
#include "Core.h"
#include <iostream>
#include <memory>
#include "VCollide\ColDetect.h"
#include <locale>
#include <valarray>
#include "easyloggingpp-9.80\src\easylogging++.h"
#include <unordered_map>
#include "Gui.h"					//zakomentovat pro noGui

//INITIALIZE_EASYLOGGINGPP


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

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


class B
{

public:
	B() : ints(vector<int>()), id(lastId++)
	{
	}
	B(const B& other) : ints(vector<int>())
	{
		for (auto a : other.as)
		{
			as.push_back(make_shared<A>(*a.get()));
		}
		ints = other.ints;
		id = other.id;
	}

	friend bool operator==(const B& lhs, const B& rhs)
	{
		return lhs.id == rhs.id;
	}

	friend bool operator!=(const B& lhs, const B& rhs)
	{
		return !(lhs == rhs);
	}

	size_t hash_value() const
	{
		size_t seed = 0x2277B8A9;
		hash_combine(seed, id);
		return seed;
	}

	static size_t hash(const B& obj)
	{
		return obj.hash_value();
	}

	vector<shared_ptr<A>> as;
	vector<int> ints;
	int id;
	static int lastId;

};

int B::lastId = 0;


class BHasher
{
public:
	size_t operator() (B const& key) const
	{
		return key.hash_value();
	}
};

class BPtrHasher
{
public:
	size_t operator() (shared_ptr<B> const& key) const
	{
		return key->hash_value();
	}
};

class EqualFn
{
public:
	bool operator() (B const& t1, B const& t2) const
	{
		return t1 == t2;
	}
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

	// testování rychlosti motion modelu kvùli optimalizaci
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
	App::InputGenerator generator(input_samples_dist, input_samples_phi);


	clock_t start;
	double duration;

	start = clock();


	//poèet všech možných "kombinací" je variace s opakováním (n-tuple anglicky). 
	//inputs jsou vstupy do modelu
	for (size_t i = 0; i < 10000; i++)
	{
		distance_of_new_nodes = 20 * (i % 10);	//10 rùzných vstupù do generátoru, abych zjistil rychlost s cache
		auto inputs = generator.generateAllInputs(distance_of_new_nodes, max_turn, near_node->uavs);	//poèet všech kombinací je poèet všech možných vstupù jednoho UAV ^ poèet UAV
		//pokud mám 10 vstupù v cache u samotného NTupleGeneratoru, je to asi 2x pomalejší než bez cache. Takže pro samotný NTupleGenerator cache nepoužívat
		//místo toho použiju cache pro celé generování vstupù
		//asi 800 ms na 1000 bìhù a 10 vstupù
		//s cache asi 300 ms na 1000 bìhù a 10 vstupù
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


	cin.get();
}

int main(int argc, char *argv[])
{
//	LOG(DEBUG) << "start of app, testing log.";
	int returnValue = 0;
//	returnValue = run(argc, argv);
	returnValue = runGui(argc, argv);
//	testing();
	return returnValue;
}

