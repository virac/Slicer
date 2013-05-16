#ifndef __RNG_H__
#define __RNG_H__

#include "Kernal\Config.h"
namespace Kernal {
class RNG {
public:
	virtual Kernal::uint32 Rand() = 0;
	virtual void Seed(Kernal::uint32 seed) = 0;

	/** Reseeds the randomizer with the clocks timestamp
	 */
	void SeedTime();

	/** Generates a random number
	 *  \param max the maximum value for the interval
	 *  \return random integer number on the interval [0,max]
	 */
	Kernal::uint32 Discreet(Kernal::uint32 max);
	/** Generates a random number
	 *  \return random number on the interval (0,1)
	 */
	double Uniform();
	/** Generates a random number
	 *  \return random number on the interval [0,1)
	 */
	double Uniform0();
	/** Generates a random number
	 *  \return random number on the interval [0,1]
	 */
	double Uniform01();
	/** Generates a random number
	 *  \return random number with a gausian distripusion with a mean of 0 and std dev of 1
	 */
	double Normal();

protected:
private:
};
} //namespace Kernal
#endif // __RNG_H__
