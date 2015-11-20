#pragma once
#include <vector>

namespace App
{

	class GuidingPathsCurrentPositions
	{
	public:
		GuidingPathsCurrentPositions();
		virtual ~GuidingPathsCurrentPositions();
		virtual int get(int index);
		virtual void set(int index, int value);
	protected:
		std::vector<int> current_index;
	};

}
