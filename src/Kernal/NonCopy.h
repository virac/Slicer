#ifndef __NONCOPY_H__
#define __NONCOPY_H__

namespace Kernal {

class NonCopy {
public:
	/** Default constructor */
	NonCopy() {
	}
private:
	/** Copy constructor
	 *  \param other Object to copy from
	 */
	NonCopy(const NonCopy& other);
	/** Assignment operator
	 *  \param other Object to assign from
	 *  \return A reference to this
	 */
	NonCopy& operator=(const NonCopy& other);
};
}

#endif // __NONCOPY_H__
