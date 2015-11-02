#include "VCollide.H"
#include "ColDetect.h"
#include "Triangle3D.h"
#include <iostream>
#include <iterator>
#include "Rectangle2D.h"

void ColDetect::addTris(double *tris, int n, VCollide &vc) {
	int id;
	double v1[3], v2[3], v3[3];
	vc.NewObject(&id);
	for (int j(0); j < n; j++) {
		v1[0] = tris[j + 0 * n];
		v1[1] = tris[j + 1 * n];
		v1[2] = tris[j + 2 * n];

		v2[0] = tris[j + 3 * n];
		v2[1] = tris[j + 4 * n];
		v2[2] = tris[j + 5 * n];

		v3[0] = tris[j + 6 * n];
		v3[1] = tris[j + 7 * n];
		v3[2] = tris[j + 8 * n];
		vc.AddTri(v1, v2, v3);
	}
	vc.EndObject();
}

void ColDetect::update(double *tr, int t, int ntrans, double vc_trans[][4]) {
	vc_trans[0][0] = tr[t + 3 * ntrans];
	vc_trans[1][0] = tr[t + 6 * ntrans];
	vc_trans[2][0] = tr[t + 9 * ntrans];
	vc_trans[3][0] = 0;
	vc_trans[0][1] = tr[t + 4 * ntrans];
	vc_trans[1][1] = tr[t + 7 * ntrans];
	vc_trans[2][1] = tr[t + 10 * ntrans];
	vc_trans[3][1] = 0;
	vc_trans[0][2] = tr[t + 5 * ntrans];
	vc_trans[1][2] = tr[t + 8 * ntrans];
	vc_trans[2][2] = tr[t + 11 * ntrans];
	vc_trans[3][2] = 0;
	vc_trans[0][3] = tr[t + 0 * ntrans];
	vc_trans[1][3] = tr[t + 1 * ntrans];
	vc_trans[2][3] = tr[t + 2 * ntrans];
	vc_trans[3][3] = 1;
}

bool ColDetect::coldetect(int ntri1, int ntri2, int ntrans1, int ntrans2, double *tri1, double *tri2, double *trans1, double *trans2) {
	double vc_trans[4][4];
	VCReport report;
	VCollide vc;
	addTris(tri1, ntri1, vc);
	addTris(tri2, ntri2, vc);
	// Iterate through transformations
	for (int t(0); t < ntrans1; t++) {
		update(trans1, t, ntrans1, vc_trans);
		vc.UpdateTrans(0, vc_trans);
		update(trans2, t, ntrans2, vc_trans);
		vc.UpdateTrans(1, vc_trans);
		vc.Collide(&report);
		if (report.numObjPairs() > 0) {
			return true;
		}
	}
	return false;
}

bool ColDetect::coldetect(Triangle3D* tri1, Triangle3D* tri2) {//wrapper, so I would not have to deal with arrays
	int ntri1 = 1;	//number of rows in first array (one row for one Triangle, propably)
	int ntri2 = 1;	//number of rows in second array (one row for one Triangle, propably)
	int ntrans1 = 1;	//number of rows for transformation matrix
	int ntrans2 = 1;	//number of rows for transformation matrix
	double zeroTrans[] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 }; //pøevzato od Petrlíka, nechápu, k èemu se tam ta matice používá
	std::vector<double> tri1vector = tri1->toVector();
	std::vector<double> tri2vector = tri2->toVector();
	double tri1array[9];
	double tri2array[9];
	std::copy(tri1vector.begin(), tri1vector.end(), tri1array);
	std::copy(tri2vector.begin(), tri2vector.end(), tri2array);
	return coldetect(ntri1, ntri2, ntrans1, ntrans2, tri1array, tri2array, zeroTrans, zeroTrans);
}

bool ColDetect::coldetect(Rectangle2D* rect1, Rectangle2D* rect2) {//splits rectangles to triangles and detects collision between all combinations of these triangles
	auto rect1triangles = rect1->toTriangles();	//propably here can be optimalization, maybe coldetect can accept more triangles
	auto rect2triangles = rect2->toTriangles();
	return coldetect(rect1triangles[0], rect2triangles[0]) ||
		coldetect(rect1triangles[0], rect2triangles[1]) ||
		coldetect(rect1triangles[1], rect2triangles[0]) ||
		coldetect(rect1triangles[1], rect2triangles[1]);
}

ColDetect::ColDetect()
{
}


ColDetect::~ColDetect()
{
}
