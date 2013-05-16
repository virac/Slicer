#include "RNG.h"

#include "Kernal\Watch.h"

#include <math.h>

namespace Kernal {

void RNG::SeedTime() {
	Kernal::uint32 t = Watch::GetTime();
	Seed(t);
}

Kernal::uint32 RNG::Discreet(Kernal::uint32 max) {
	return Kernal::uint32(Uniform0() * ((double) (max + 1)));
}
double RNG::Uniform() {
	return (Rand() + 1) / ((double) UINT32_MAX + 2); //not sure on this
}
double RNG::Uniform0() {
	return Rand() / ((double) UINT32_MAX + 1);
}
double RNG::Uniform01() {
	return Rand() / ((double) UINT32_MAX);
}
double RNG::Normal() {
	double d1 = Uniform();
	double d2 = Uniform();
	double r = sqrt(-2.0 * log(d1));
	double theta = 2.0 * PI * d2;
	return r * sin(theta);
}

} // namespace Kernal

