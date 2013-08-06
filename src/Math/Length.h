/*
 * Length.h
 *
 *  Created on: May 28, 2013
 *      Author: Jon
 */

#ifndef LENGTH_H_
#define LENGTH_H_

#define INCHES_TO_MM 25.4
#define INCHES_TO_FEET 12
#define FEET_TO_YARD 3
#define MM_TO_CM 10
#define CM_TO_M 100

namespace Math {

enum LENGTH_UNIT {
	LENGTH_UNIT_MM,
	LENGTH_UNIT_CM,
	LENGTH_UNIT_M,
	LENGTH_UNIT_IN,
	LENGTH_UNIT_FT,
	LENGTH_UNIT_PERCENT,
	LENGTH_UNIT_NONE
};
class Length {
public:
	Length( double valueIn = 1.0, LENGTH_UNIT valueUnitIn = LENGTH_UNIT_M);

	void setBaseUnit( LENGTH_UNIT typeIn );
	LENGTH_UNIT getUnit();
	double convert(LENGTH_UNIT typeIn = LENGTH_UNIT_NONE, double valueIn = 0);
	void setValue(double valueIn, LENGTH_UNIT typeIn = LENGTH_UNIT_NONE);
private:
	double toBase();
	double value;
	LENGTH_UNIT valueUnit;
};


} /* namespace Math */
#endif /* LENGTH_H_ */
