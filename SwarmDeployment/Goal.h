#pragma once
#include "Rectangle.h"
#include "GoalInterface.h"

using namespace std;

namespace App
{

	class Goal : public GoalInterface
	{

	public:
		Goal(int x, int y, int width, int height);
		virtual ~Goal();
		virtual bool contains(shared_ptr<Point> location) override;
		friend bool operator==(const Goal& lhs, const Goal& rhs);
		friend bool operator!=(const Goal& lhs, const Goal& rhs);
		virtual shared_ptr<Point> getRandomPointInside() override;
		friend size_t hash_value(const Goal& obj);
		virtual shared_ptr<Rectangle> getRectangle() override;	//není const, protože GoalGroup implementace GoalInterfacu se zavoláním getteru lazy inicializuje. po vytvoøení builderu bude opìt moct být lazy

	protected:
		shared_ptr<Rectangle> rectangle;
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