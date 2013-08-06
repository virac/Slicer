/*
 * Angle.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Jon
 */

#include "Angle.h"

namespace Math {

Angle::Angle(double valueIn, ANGLE_UNIT valueUnitIn) :
		value(valueIn), valueUnit(valueUnitIn) {
}

void Angle::setBaseUnit(ANGLE_UNIT typeIn) {
	this->value = this->convert(typeIn);
	this->valueUnit = typeIn;
}

ANGLE_UNIT Angle::getUnit() {
	return this->valueUnit;
}

double Angle::convert(ANGLE_UNIT typeIn) {
	if (typeIn == this->valueUnit || typeIn == ANGLE_UNIT_NONE) {
		return this->value;
	}

	if (typeIn == ANGLE_UNIT_DEGREES) {
		return this->value * RADIANS_TO_DEGREES;
	} else {
		return this->value * DEGREES_TO_RADIANS;
	}
}

void Angle::setValue(double valueIn, ANGLE_UNIT typeIn) {
	if (typeIn != ANGLE_UNIT_NONE) {
		this->valueUnit = typeIn;
	}
	this->value = valueIn;
}

} /* namespace Math */
