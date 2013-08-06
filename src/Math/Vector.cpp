/*
 * Vector.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: Jon
 */

#include "Vector.h"
#include <math.h>

namespace Math {

Vector::Vector() :
		x(0.0), y(0.0), z(0.0) {
}
Vector::Vector(Vector& v) :
		x(v.x), y(v.x), z(v.z) {
}
Vector::Vector(Vector *v) :
x(v->x), y(v->x), z(v->z) {
}
Vector::Vector(double x_In, double y_In, double z_In) :
		x(x_In), y(y_In), z(z_In) {
}
double Vector::dot(Vector& rhs) {
	return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}
void Vector::cross(Vector& rhs, Vector& normal) {
	normal.x = ((this->y * rhs.z) - (this->z * rhs.y));
	normal.y = ((this->z * rhs.x) - (this->x * rhs.z));
	normal.z = ((this->x * rhs.y) - (this->y * rhs.x));
}


Vector* Vector::minus( Vector& rhs ){
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return this;
}
Vector* Vector::add( Vector& rhs ){
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return this;
}
Vector* Vector::scale( double rhs ){
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return this;
}
/*
Vector Vector::operator +(const Math::Vector vector) {
	return Vector(vector.x + x, vector.y + y, vector.z + z);
}
Vector Vector::operator -(const Vector vector) {
	return Vector(vector.x - x, vector.y - y, vector.z - z);
}
Vector Vector::operator *(float num) {
	return Vector(x * num, y * num, z * num);
}
Vector Vector::operator /(float num) {
	return Vector(x / num, y / num, z / num);
}*/

double Vector::magnitude() {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}
void Vector::normalize() {
	double mag = this->magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}
void Vector::zeroize() {
	this->x = this->y = this->z = 0.0;
}

bool Vector::aboutEqual(const Vector& rhs) {
	return ((abs(this->x - rhs.x) < EPSILON) && (abs(this->y - rhs.y) < EPSILON)
			&& (abs(this->z - rhs.z) < EPSILON));
}

bool operator==(const Vector& lhs, const Vector& rhs) {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}
bool operator!=(const Vector& lhs, const Vector& rhs) {
	return !operator ==(lhs, rhs);
}

bool operator<(const Vector& lhs, const Vector& rhs) {
	return ((lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y))
			|| ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z < rhs.z)));
}
bool operator>(const Vector& lhs, const Vector& rhs) {
	return ((lhs.x > rhs.x) || ((lhs.x == rhs.x) && (lhs.y > rhs.y))
			|| ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z > rhs.z)));
}
bool VectorPointerLess::operator ()(const Vector *lhs, const Vector *rhs) {
	return *lhs < *rhs;
}

double Vector::getX() const {
	return x;
}

void Vector::setX(double x) {
	this->x = x;
}

double Vector::getY() const {
	return y;
}

void Vector::setY(double y) {
	this->y = y;
}

double Vector::getZ() const {
	return z;
}

void Vector::setZ(double z) {
	this->z = z;
}

}
