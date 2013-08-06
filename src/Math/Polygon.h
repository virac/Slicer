/*
 * Polygon.h
 *
 *  Created on: Jun 9, 2013
 *      Author: Jon
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Math\Vector.h"

namespace Math {
class Polygon {
public:
	Polygon() {
		this->normal = NULL;
		this->verticies = NULL;
	}
	Polygon(Math::VectorArray& v_In) {
		this->verticies = new Math::VectorPtrArray();
		for( Math::VectorArray::iterator i = v_In.begin(); i < v_In.end(); i++ ) {
			Math::Vector *newVertex = NULL;
			newVertex = new Math::Vector(*i);
			this->verticies->push_back(newVertex);
		}
		this->normal = NULL;
		this->generateNormal();
	}
	Polygon(Math::VectorPtrArray& v_In) {
		this->verticies = new Math::VectorPtrArray();
		for( Math::VectorPtrArray::iterator i = v_In.begin(); i < v_In.end(); i++ ) {
			this->verticies->push_back(*i);
		}
		this->normal = NULL;
		this->generateNormal();
	}
	~Polygon() {
		if( this->normal != NULL ){
			delete this->normal;
		}
	}

	Math::Vector* getNormal()  {
		return normal;
	}

	Math::VectorPtrArray *getVerticies() {
		return verticies;
	}

protected:
	void generateNormal() {
		/* http://www.opengl.org/wiki/Calculating_a_Surface_Normal
		 * Begin Function CalculateSurfaceNormal (Input Polygon) Returns Vector
		 * 	Set Vertex Normal to (0, 0, 0)
		 * 		Begin Cycle for Index in [0, Polygon.vertexNumber)
		 * 		Set Vertex Current to Polygon.verts[Index]
		 * 			Set Vertex Next    to Polygon.verts[(Index plus 1) mod Polygon.vertexNumber]
		 *
		 * 			Set Normal.x to Sum of Normal.x and (multiply (Current.y minus Next.y) by (Current.z plus Next.z))
		 * 			Set Normal.y to Sum of Normal.y and (multiply (Current.z minus Next.z) by (Current.x plus Next.x))
		 * 			Set Normal.z to Sum of Normal.z and (multiply (Current.x minus Next.x) by (Current.y plus Next.y))
		 * 		End Cycle
		 * 	Returning Normalize(Normal)
		 * End Function
		 * */
		if( this->normal == NULL ){
			this->normal = new Vector;
		}
		this->normal->zeroize();

		double normalx = 0.0;
		double normaly = 0.0;
		double normalz = 0.0;
		if (verticies->size() < 3) {
			return;
		}
		for (Math::VectorPtrArray::iterator i = this->verticies->begin();
				i != this->verticies->end(); i++) {
			Math::VectorPtrArray::iterator j = i + 1;
			if (j == this->verticies->end()) {
				j = this->verticies->begin();
			}

			normalx += ((*i)->getY() - (*j)->getY()) * ((*i)->getZ() + (*j)->getZ());
			normaly += ((*i)->getZ() - (*j)->getZ()) * ((*i)->getX() + (*j)->getX());
			normalz += ((*i)->getX() - (*j)->getX()) * ((*i)->getY() + (*j)->getY());
		}
		normal->setX(normalx);
		normal->setY(normaly);
		normal->setZ(normalz);
		normal->normalize();
	}
	;
	Math::Vector *normal; // facet surface normal
	Math::VectorPtrArray *verticies;

};
typedef std::vector<Polygon> PolygonArray;
typedef std::vector<Polygon*> PolygonPtrArray;
}
#endif /* POLYGON_H_ */
