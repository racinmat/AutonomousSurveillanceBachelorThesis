#include "Persister.h"
#include <fstream>
//include <boost/algorithm/string.hpp>
//include <boost/algorithm/string/split.hpp>

namespace App
{

	Persister::Persister()
	{
	}


	Persister::~Persister()
	{
	}

	void Persister::savePath(vector<shared_ptr<State>> path)
	{
		ofstream file = ofstream("path" + to_string(time(nullptr)) +  ".txt");
		for (auto state : path) {
			file << *state << ", " << endl;
		}
		cout << endl;
		file.close();
	}

	vector<shared_ptr<State>> Persister::loadPath()
	{
//		ifstream file("path.txt");
//		string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
//		vector<string> states;
//		split(states, content, boost::is_any_of(",\nuavs: \n"));

		//todo: implement
		return vector<shared_ptr<State>>();
	}

}
