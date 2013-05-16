#include "RNGMarsagliaSimple.h"
namespace Kernal {
RNGMarsagliaSimple::RNGMarsagliaSimple() {
	SeedTime();
}

RNGMarsagliaSimple::RNGMarsagliaSimple(const RNGMarsagliaSimple& other) {
	state = other.state;
}

RNGMarsagliaSimple& RNGMarsagliaSimple::operator=(
		const RNGMarsagliaSimple& rhs) {
	state = rhs.state;
	return *this;
}

Kernal::uint32 RNGMarsagliaSimple::Rand() {
	return state = (69069 * state) + 362437;
}

void RNGMarsagliaSimple::Seed(Kernal::uint32 seed) {
	state = seed;
}
} //namespace Kernal

