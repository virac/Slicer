/*
 * Quaternion.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: Jon
 */

#include "Quaternion.h"
#include <math.h>

namespace Math {

Quaternion::Quaternion() :
		x(0.0), y(0.0), z(0.0), w(0.0) {
}

Quaternion::Quaternion(Quaternion* q_In) {
	this->x = q_In->x;
	this->y = q_In->y;
	this->z = q_In->z;
	this->w = q_In->w;
}

Quaternion::Quaternion(Math::Vector* axis, Math::Angle* angle) :
				x(0.0), y(0.0), z(0.0), w(0.0) {
	this->set(axis, angle);
}

Quaternion::Quaternion(double x_In, double y_In, double z_In, double w_In) {
	this->x = x_In;
	this->y = y_In;
	this->z = z_In;
	this->w = w_In;
}

Quaternion::~Quaternion() {
}

double Quaternion::getW() const {
	return w;
}

void Quaternion::setW(double w) {
	this->w = w;
}

double Quaternion::getX() const {
	return x;
}

void Quaternion::setX(double x) {
	this->x = x;
}

double Quaternion::getY() const {
	return y;
}

void Quaternion::setY(double y) {
	this->y = y;
}

double Quaternion::getZ() const {
	return z;
}

void Quaternion::setZ(double z) {
	this->z = z;
}

void Quaternion::conjugate(Quaternion* q1) {
	this->x = -q1->x;
	this->y = -q1->y;
	this->z = -q1->z;
	this->w = q1->w;
}

Quaternion* Quaternion::conjugate() {
	Quaternion *q1 = new Quaternion(-this->x, -this->y, -this->z, this->w);
	return q1;
}

void Quaternion::add(Quaternion* q1) {
	this->x += q1->x;
	this->y += q1->y;
	this->z += q1->z;
	this->w += q1->w;
}

void Quaternion::multiply(Quaternion* q1) {
	this->x = this->x * q1->w + this->y * q1->z - this->z * q1->y
			+ this->w * q1->x;
	this->y = -this->x * q1->z + this->y * q1->w + this->z * q1->x
			+ this->w * q1->y;
	this->z = this->x * q1->y - this->y * q1->x + this->z * q1->w
			+ this->w * q1->z;
	this->w = -this->x * q1->x - this->y * q1->y - this->z * q1->z
			+ this->w * q1->w;
}

void Quaternion::scale(double s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
}

void Quaternion::normalize() {
	double n = sqrt(
			(this->x * this->x) + (this->y * this->y) + (this->z * this->z)
					+ (this->w * this->w));
	this->x /= n;
	this->y /= n;
	this->z /= n;
	this->w /= n;
}

Math::Vector* Quaternion::transform(Math::Vector* p1) {
	double p1x = p1->getX();
	double p1y = p1->getY();
	double p1z = p1->getZ();

	double p2x = 0.0;
	double p2y = 0.0;
	double p2z = 0.0;

	double xSquared = this->x * this->x;
	double ySquared = this->y * this->y;
	double zSquared = this->z * this->z;
	double wSquared = this->w * this->w;

	double xy2 = 2 * this->x * this->y;
	double xz2 = 2 * this->x * this->z;
	double xw2 = 2 * this->x * this->w;
	double yw2 = 2 * this->y * this->w;
	double zw2 = 2 * this->z * this->w;

	p2x = wSquared * p1x + yw2 * p1z - zw2 * p1y + xSquared * p1x + xy2 * p1y
			+ xz2 * p1z - zSquared * p1x - ySquared * p1x;
	p2y = xy2 * p1x + ySquared * p1y + 2 * ySquared * p1z + zw2 * p1x
			- zSquared * p1y + wSquared * p1y - xw2 * p1z - xSquared * p1y;
	p2z = xz2 * p1x + 2 * y * z * p1y + zSquared * p1z - yw2 * p1x
			- ySquared * p1z + xw2 * p1y - xSquared * p1z + wSquared * p1z;
	/*
	 * p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y + 2*z*x*p1.z - z*z*p1.x - y*y*p1.x;
	 * p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y;
	 * p2.z = 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y - x*x*p1.z + w*w*p1.z;
	 */
	Math::Vector *p2 = new Math::Vector(p2x,p2y,p2z);
	return p2;
}

void Quaternion::set(Math::Vector* axis, Math::Angle* angle) {
	double angleRadsHalf = angle->convert(Math::ANGLE_UNIT_RADIANS) / 2.0;
	double sin_angleRadsHalf = sin(angleRadsHalf);
	this->x = axis->getX() * sin_angleRadsHalf;
	this->y = axis->getY() * sin_angleRadsHalf;
	this->z = axis->getZ() * sin_angleRadsHalf;
	this->w = cos(angleRadsHalf);
}

} /* namespace Math */
