/*
 * Length.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Jon
 */

#include "Length.h"

namespace Math {

Length::Length(double valueIn, LENGTH_UNIT valueUnitIn) :
		value(valueIn), valueUnit(valueUnitIn) {
}
void Length::setBaseUnit(LENGTH_UNIT typeIn) {
	this->value = this->convert( typeIn );
	this->valueUnit = typeIn;
}

LENGTH_UNIT Length::getUnit() {
	return this->valueUnit;
}

double Length::convert(LENGTH_UNIT typeIn, double valueIn) {
	if (typeIn == LENGTH_UNIT_PERCENT
			|| this->valueUnit == LENGTH_UNIT_PERCENT) {
		return this->value * valueIn;
	}
	if (typeIn == this->valueUnit || typeIn == LENGTH_UNIT_NONE) {
		return this->value;
	}
	double baseVal = this->value;
	if (this->valueUnit != LENGTH_UNIT_M) {
		baseVal = this->toBase();
	}
	switch (typeIn) {
	case LENGTH_UNIT_MM:
		return baseVal / CM_TO_M / MM_TO_CM;
		break;
	case LENGTH_UNIT_CM:
		return baseVal / CM_TO_M;
		break;
	case LENGTH_UNIT_IN:
		return baseVal / CM_TO_M / MM_TO_CM / INCHES_TO_MM;
		break;
	case LENGTH_UNIT_FT:
		return baseVal / CM_TO_M / MM_TO_CM / INCHES_TO_MM * INCHES_TO_FEET;
		break;
	case LENGTH_UNIT_PERCENT: //shouldnt get here but in to get rid of warning
	case LENGTH_UNIT_NONE: // here either
	case LENGTH_UNIT_M:
	default:
		break;
	}
	return baseVal;
}

void Length::setValue(double valueIn, LENGTH_UNIT typeIn) {
	if (typeIn != LENGTH_UNIT_NONE) {
		this->valueUnit = typeIn;
	}
	this->value = valueIn;
}

double Length::toBase() {
	switch (this->valueUnit) {
	case LENGTH_UNIT_MM:
		return this->value * MM_TO_CM * CM_TO_M;
		break;
	case LENGTH_UNIT_CM:
		return this->value * CM_TO_M;
		break;
	case LENGTH_UNIT_IN:
		return this->value * INCHES_TO_MM * MM_TO_CM * CM_TO_M;
		break;
	case LENGTH_UNIT_FT:
		return this->value / INCHES_TO_FEET * INCHES_TO_MM * MM_TO_CM * CM_TO_M;
		break;
	case LENGTH_UNIT_M:
	case LENGTH_UNIT_PERCENT:
	case LENGTH_UNIT_NONE:
	default:
		break;
	}
	return this->value;
}

}
