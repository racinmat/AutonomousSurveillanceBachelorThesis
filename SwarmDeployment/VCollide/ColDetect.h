#pragma once
#include "Triangle3D.h"
#include "Rectangle2D.h"
#include "VCollide.H"

class ColDetect
{
public:
	bool coldetect(Triangle3D* tri1, Triangle3D* tri2);
	bool coldetect(Rectangle2D* tri1, Rectangle2D* tri2);
	ColDetect();
	virtual ~ColDetect();

protected:
	void addTris(double* tris, int n, VCollide& vc);
	bool coldetect(int ntri1, int ntri2, int ntrans1, int ntrans2, double* tri1, double* tri2, double* trans1, double* trans2);
	void update(double *tr, int t, int ntrans, double vc_trans[][4]);
};

