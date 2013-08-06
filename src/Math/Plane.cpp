/*
 * Plane.cpp
 *
 *  Created on: May 19, 2013
 *      Author: Jon
 */

#include "Plane.h"

#include <math.h>
#include <cstddef>
#include <set>

#include "Kernal\Log.h"

namespace Math {

Plane::~Plane() {
	if (this->normal != NULL) {
		delete this->normal;
	}
}

double Plane::distanceFromPlane(Vector *p) {
	return this->normal->dot(*p) + this->dist;
}

void Plane::getSegmentPlaneIntersection(Vector *p1, Vector *p2,
		VectorPtrArray *intersections, double p1Dot, double p2Dot) {
	double d1 = (p1Dot == 0.0f ? distanceFromPlane(p1) : p1Dot);
	double d2 = (p2Dot == 0.0f ? distanceFromPlane(p2) : p2Dot);
	bool onPlaneP1 = abs(d1) < this->eps;
	bool onPlaneP2 = abs(d2) < this->eps;
	if (onPlaneP1) {
		addNewIfNotAlrdy(intersections,p1);
	}
	if (onPlaneP2) {
		addNewIfNotAlrdy(intersections,p2);
	}
	if ((onPlaneP1 && onPlaneP2) || (d1 * d1 > eps)) {
		return;
	}
	double d = d1 / (d1 - d2);

//	Vector dp21 = *p2 - *p1;
//	intersections->push_back(new Vector(*p1 + (dp21 * d)) );

	Vector *dp21 = new Vector(p2);
	dp21->minus(*p1);

	dp21->scale(d)->add(*p1);

	intersections->push_back( dp21 );
}

void Plane::getTrianglePlaneIntersection(Vector *p1, Vector *p2, Vector *p3,
		VectorPtrArray *intersections, double p1Dot, double p2Dot,
		double p3Dot) {
	this->getSegmentPlaneIntersection(p1, p2, intersections, p1Dot, p2Dot);
	this->getSegmentPlaneIntersection(p2, p3, intersections, p2Dot, p3Dot);
	this->getSegmentPlaneIntersection(p3, p1, intersections, p3Dot, p1Dot);
//	this->removeDuplicates(intersections);
}

void Plane::addNewIfNotAlrdy(VectorPtrArray *intersections, Vector *p) {
	VectorPtrArray::iterator i;
	for (i = intersections->begin(); i < intersections->end(); i++) {
		if (**i == *p) {
			return;
		}
	}
	if (i == intersections->end()) {
		intersections->push_back(new Vector(*p));
	}
	/*	std::set<Vector> s;
	 unsigned size = array->size();
	 for (unsigned i = 0; i < size; ++i) {
	 s.insert((*array)[i]);
	 }
	 array->assign(s.begin(), s.end());*/
}

}
/* namespace Math */
