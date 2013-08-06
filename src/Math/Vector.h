/*
 * Vector
 *
 *  Created on: May 18, 2013
 *      Author: Jon
 */

#ifndef __VECTOR64_H__
#define __VECTOR64_H__

#include <vector>

#include "Kernal\config.h"

namespace Math {
class Vector {
public:
	Vector();
	Vector(Vector& v);
	Vector(Vector* v);
	Vector(double x_In, double y_In, double z_In);
	double dot(Vector& rhs);
	void cross(Vector& rhs, Vector& normal);

	Vector*  minus( Vector& rhs );
	Vector*  add( Vector& rhs );
	Vector*  scale( double rhs );
//	Vector operator +(const Vector vector);
//	Vector operator -(const Vector vector);
//	Vector operator *(float num);
//	Vector operator /(float num);

	double magnitude();
	void normalize();
	void zeroize();
	bool aboutEqual(const Vector& rhs);
	friend bool operator ==(const Vector& lhs, const Vector& rhs);
	friend bool operator !=(const Vector& lhs, const Vector& rhs);
	friend bool operator <(const Vector& lhs, const Vector& rhs);
	friend bool operator >(const Vector& lhs, const Vector& rhs);
	double getX() const;
	void setX(double x);
	double getY() const;
	void setY(double y);
	double getZ() const;
	void setZ(double z);

private:
	double x;
	double y;
	double z;
};

struct VectorPointerLess {
	bool operator ()(const Vector *lhs, const Vector *rhs);
};
typedef std::vector<Vector> VectorArray;
typedef std::vector<Vector*> VectorPtrArray;
}

#endif /* __VECTOR64_H__ */
