/*
 * Angle.h
 *
 *  Created on: May 30, 2013
 *      Author: Jon
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include "Kernal\Config.h"

#define DEGREES_TO_RADIANS PI / 180.0
#define RADIANS_TO_DEGREES 1.0 / DEGREES_TO_RADIANS


namespace Math {

enum ANGLE_UNIT {
	ANGLE_UNIT_DEGREES,
	ANGLE_UNIT_RADIANS,
	ANGLE_UNIT_NONE
};

class Angle {
public:
	Angle(double valueIn = 0.0, ANGLE_UNIT valueUnitIn = ANGLE_UNIT_RADIANS);

	void setBaseUnit(ANGLE_UNIT typeIn);
	ANGLE_UNIT getUnit();
	double convert(ANGLE_UNIT typeIn = ANGLE_UNIT_NONE);
	void setValue(double valueIn, ANGLE_UNIT typeIn = ANGLE_UNIT_NONE);
private:
	double value;
	ANGLE_UNIT valueUnit;
};

} /* namespace Math */
#endif /* ANGLE_H_ */
