#pragma once
#include "Rectangle.h"
#include "vector"

using namespace std;

namespace App
{

	class Goal
	{

	public:
		Goal(int x, int y, int width, int height);
		virtual ~Goal();
		std::shared_ptr<Rectangle> rectangle; //todo: pøesunout do private
		bool contains(shared_ptr<Point> location);
		friend bool operator==(const Goal& lhs, const Goal& rhs);
		friend bool operator!=(const Goal& lhs, const Goal& rhs);
		friend std::size_t hash_value(const Goal& obj);
	};

	class GoalHasher
	{
	public:
		size_t operator() (Goal const& key) const
		{
			return hash_value(key);
		}
	};

}