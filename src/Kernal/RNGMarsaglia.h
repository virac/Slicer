#ifndef __RNG_MARSAGLIA_H__
#define __RNG_MARSAGLIA_H__

#include "RNG.h"

namespace Kernal {
class RNGMarsaglia: public RNG {
public:
	/** Default constructor */
	RNGMarsaglia();
	/** Copy constructor
	 *  \param other RNGMarsaglia to copy from
	 */
	RNGMarsaglia(const RNGMarsaglia& other);
	/** Assignment operator
	 *  \param other RNGMarsaglia to assign from
	 *  \return A reference to this
	 */
	RNGMarsaglia& operator=(const RNGMarsaglia& other);

	virtual Kernal::uint32 Rand();
	virtual void Seed(Kernal::uint32 seed);
protected:
private:
	Kernal::uint32 stateZ; //!< Member variable "state1"
	Kernal::uint32 stateW; //!< Member variable "state2"
};
}
#endif // __RNG_MARSAGLIA_H__
