/*
 * Settings.h
 *
 *  Created on: May 28, 2013
 *      Author: Jon
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Math/Length.h"
#include "Math/Angle.h"

enum SQUARE_UNIT {
	MM2, IN2
};

enum FILL_PATTERN {
	RECTALINEAR, LINE, CONCENTRIC, HONEYCOMB, HILBERT_CURVE
};

class Settings {
public:
	Settings();
	~Settings();

	Math::Length layerHeight;
	Math::Length layerHeightFirst;
	int perimeters;
	bool perimeterRandomStart;
	bool perimeterExtraAllowed;
	int shellsTop;
	int shellsBottom;

	double infil;
	FILL_PATTERN infilPattern;
	FILL_PATTERN infilPatternSolid;
	FILL_PATTERN infilPatternTopBottom;
	int infilPeriod;
	int infilPeriodSolid;
	Math::Angle infilAngleUnit;
	double infilSolidThreshold;
	SQUARE_UNIT infilSolidThresholdUnit;
private:
};

#endif /* SETTINGS_H_ */
