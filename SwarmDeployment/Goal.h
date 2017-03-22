#pragma once
#include "Rectangle.h"
#include "GoalInterface.h"

using namespace std;

namespace App
{

	class Goal : public GoalInterface, public enable_shared_from_this<Goal>
	{

	public:
		Goal(int x, int y, int width, int height);
		Goal(shared_ptr<Rectangle> rectangle);
		virtual ~Goal();
		virtual bool contains(shared_ptr<Point> location) override;
		friend bool operator==(const Goal& lhs, const Goal& rhs);
		friend bool operator!=(const Goal& lhs, const Goal& rhs);
		virtual shared_ptr<Point> getRandomPointInside() override;
		friend size_t hash_value(const Goal& obj);
		static shared_ptr<Goal> fromJson(Value& data);
		virtual shared_ptr<Rectangle> getRectangle() override;	//nen� const, proto�e GoalGroup implementace GoalInterfacu se zavol�n�m getteru lazy inicializuje. po vytvo�en� builderu bude op�t moct b�t lazy
		virtual shared_ptr<Goal> getConcreteGoal(shared_ptr<Point> location) override;
		virtual shared_ptr<Point> getMiddle() override;

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