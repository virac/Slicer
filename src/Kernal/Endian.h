#ifndef __ENDIAN_H__
#define __ENDIAN_H__

namespace Kernal {

void InitEndian(void);

short ShortSwap(short s);
short ShortNoSwap(short s);

int IntSwap(int i);
int IntNoSwap(int i);

float FloatSwap(float f);
float FloatNoSwap(float f);

extern short (*FromBigShort)(short s);
extern short (*FromLittleShort)(short s);
extern int (*FromBigInt)(int i);
extern int (*FromLittleInt)(int i);
extern float (*FromBigFloat)(float f);
extern float (*FromLittleFloat)(float f);

extern bool BigEndianSystem;  //you might want to extern this
}
#endif
