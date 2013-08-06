/*
 * Endian.cpp
 *
 *  Created on: May 24, 2013
 *      Author: Jon
 */

#include "Kernal\Endian.h"
namespace Kernal {
typedef unsigned char byte;

bool BigEndianSystem;

short (*FromBigShort)(short s);
short (*FromLittleShort)(short s);
int (*FromBigInt)(int i);
int (*FromLittleInt)(int i);
float (*FromBigFloat)(float f);
float (*FromLittleFloat)(float f);

//adapted from Quake 2 source

short ShortSwap(short s) {
	byte b1, b2;

	b1 = s & 255;
	b2 = (s >> 8) & 255;

	return (b1 << 8) + b2;
}

short ShortNoSwap(short s) {
	return s;
}

int IntSwap(int i) {
	byte b1, b2, b3, b4;

	b1 = i & 255;
	b2 = (i >> 8) & 255;
	b3 = (i >> 16) & 255;
	b4 = (i >> 24) & 255;

	return ((int) b1 << 24) + ((int) b2 << 16) + ((int) b3 << 8) + b4;
}

int IntNoSwap(int i) {
	return i;
}

float FloatSwap(float f) {
	union {
		float f;
		byte b[4];
	} dat1, dat2;

	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}

float FloatNoSwap(float f) {
	return f;
}

void InitEndian(void) {
	//clever little trick from Quake 2 to determine the endian
	//of the current system without depending on a preprocessor define

	byte SwapTest[2] = { 1, 0 };

	if (*(short *) SwapTest == 1) {
		//little endian
		BigEndianSystem = false;

		//set func pointers to correct funcs
		FromBigShort = ShortSwap;
		FromLittleShort = ShortNoSwap;
		FromBigInt = IntSwap;
		FromLittleInt = IntNoSwap;
		FromBigFloat = FloatSwap;
		FromLittleFloat = FloatNoSwap;
	} else {
		//big endian
		BigEndianSystem = true;

		FromBigShort = ShortNoSwap;
		FromLittleShort = ShortSwap;
		FromBigInt = IntNoSwap;
		FromLittleInt = IntSwap;
		FromBigFloat = FloatSwap;
		FromLittleFloat = FloatNoSwap;
	}
}
}
