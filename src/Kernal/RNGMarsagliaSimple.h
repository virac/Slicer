#ifndef __RNG_MARSAGLIA_SIMPLE_H__
#define __RNG_MARSAGLIA_SIMPLE_H__

#include "RNG.h"

namespace Kernal {
class RNGMarsagliaSimple: public RNG {
public:
	/** Default constructor */
	RNGMarsagliaSimple();
	/** Copy constructor
	 *  \param other RNGMarsagliaSimple to copy from
	 */
	RNGMarsagliaSimple(const RNGMarsagliaSimple& other);
	/** Assignment operator
	 *  \param other RNGMarsagliaSimple to assign from
	 *  \return A reference to this
	 */
	RNGMarsagliaSimple& operator=(const RNGMarsagliaSimple& other);

	virtual Kernal::uint32 Rand();
	virtual void Seed(Kernal::uint32 seed);
protected:
private:
	uint32 state; //!< Member variable "state"
};
} //namespace Kernal
#endif // __RNG_MARSAGLIA_H__
