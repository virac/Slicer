/*
 * Triangle.h
 *
 *  Created on: Jun 9, 2013
 *      Author: Jon
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Math\Polygon.h"
#include "Math\Vector.h"

namespace Math {
class Triangle : Math::Polygon {
public:
	Triangle(Vector *v1_In, Vector *v2_In, Vector *v3_In) {
		this->normal = NULL;
		this->verticies = new Math::VectorPtrArray();
		this->verticies->push_back( v1_In );
		this->verticies->push_back( v2_In );
		this->verticies->push_back( v3_In );

		this->generateNormal();
	}
private:

};

typedef std::vector<Triangle> TriangleArray;
typedef std::vector<Triangle*> TrianglePtrArray;
}
#endif /* TRIANGLE_H_ */
