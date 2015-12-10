#include "Persister.h"
#include <fstream>


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
		ofstream file = ofstream("path.txt");
		for (auto state : path) {
			file << *state << ", " << endl;
		}
		cout << endl;
		file.close();
	}

	vector<shared_ptr<State>> Persister::loadPath()
	{
		//todo: implement
		return vector<shared_ptr<State>>();
	}

}
