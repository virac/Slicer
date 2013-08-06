/*
 * Quaternion.h
 *
 *  Created on: Jun 11, 2013
 *      Author: Jon
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Math\Vector.h"
#include "Math\Angle.h"

namespace Math {

class Quaternion {
public:
	Quaternion();
	Quaternion( Quaternion *q_In );
	Quaternion( Math::Vector *axis, Math::Angle *angle );
	Quaternion( double x, double y, double z, double w );
	~Quaternion();

	void conjugate( Quaternion *q1 );
	Quaternion* conjugate();
	void add( Quaternion *q1 );
	void multiply( Quaternion *q1 );
	void scale( double s );
	void normalize();

	Math::Vector* transform( Math::Vector *p1 );

	void set( Math::Vector *axis, Math::Angle *angle );

	double getW() const;
	void setW(double w);
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
	double w;
};

} /* namespace Math */
#endif /* QUATERNION_H_ */
