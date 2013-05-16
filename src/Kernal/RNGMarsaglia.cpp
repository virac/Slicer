#include "RNGMarsaglia.h"

namespace Kernal {
RNGMarsaglia::RNGMarsaglia() {
	SeedTime();
}

RNGMarsaglia::RNGMarsaglia(const RNGMarsaglia& other) {
	stateZ = other.stateZ;
	stateW = other.stateW;
}

RNGMarsaglia& RNGMarsaglia::operator=(const RNGMarsaglia& rhs) {
	stateZ = rhs.stateZ;
	stateW = rhs.stateW;

	return *this;
}

Kernal::uint32 RNGMarsaglia::Rand() {
	stateZ = 36969 * (stateZ & 65535) + (stateZ >> 16);
	stateW = 18000 * (stateW & 65535) + (stateW >> 16);
	return (stateZ << 16) + stateW;
}

void RNGMarsaglia::Seed(Kernal::uint32 seed) {
	stateZ = seed >> 16;
	stateW = seed & UINT16_MAX; // % TWO_TO_32
}
} //namespace Kernal

