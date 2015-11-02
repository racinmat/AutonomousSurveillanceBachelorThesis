#pragma once
class Point3D
{
public:
	Point3D(double x, double y, double z);
	virtual ~Point3D();
	double getX() const;
	double getY() const;
	double getZ() const;

private:
	double x;
	double y;
	double z;
};

