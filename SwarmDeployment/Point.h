#pragma once

namespace App
{
	class Point
	{
	public:
		Point(double x, double y);
		Point(double x, double y, double z);
		~Point();
		double getX() const;
		void setX(double x);
		double getY() const;
		void setY(double y);
		void changeX(double x);
		void changeY(double y);
		virtual double getZ() const;
		virtual void setZ(const double z);
		void changeZ(double z);

	protected:
		double x;
		double y;
		double z;
	};

}

