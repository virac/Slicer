/*
 * Plane.h
 *
 *  Created on: May 19, 2013
 *      Author: Jon
 */

#ifndef __PLANE_H__
#define __PLANE_H__

#include "Vector.h"
#include "Kernal\config.h"

namespace Math {

class Plane {
public:
	Plane(Vector *normal_In, double dist_In, double eps_In = EPSILON) :
			normal(normal_In), dist(dist_In), eps(eps_In) {
	}
	~Plane();

	double distanceFromPlane(Vector *p);
	void getSegmentPlaneIntersection(Vector *p1, Vector *p2,
			VectorPtrArray *intersections, double p1Dot = 0.0, double p2Dot = 0.0);
	void getTrianglePlaneIntersection(Vector *p1, Vector *p2, Vector* p3,
			VectorPtrArray *intersections, double p1Dot = 0.0, double p2Dot = 0.0,
			double p3Dot = 0.0);
private:
	Vector *normal;
	double dist;
	double eps;

	void addNewIfNotAlrdy(VectorPtrArray * array, Vector *p);
};

} /* namespace Math */
#endif /* __PLANE_H__ */
