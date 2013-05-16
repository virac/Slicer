#ifndef __WATCH_H__
#define __WATCH_H__

#include "Config.h"

#ifdef SYSTEM_WINDOWS
#include <windows.h>
#elif defined SYSTEM_LINUX
typedef LONGLONG LARGE_INTEGER;
#endif

namespace Kernal {
class Watch {
public:
	/** Default constructor */
	Watch();

	/** Gets the time from when the Watch was last reset or created.
	 *  \return Time elapsed
	 */
	double GetSplit() const;
	/** Resets the watch to the current time.
	 */
	void Reset();
	static void SleepSec(float time);
	/** Gets the current time from the system.
	 *  \return Current time in mSecs
	 */
	static Kernal::uint32 GetTime();
protected:
private:
	/** Gets the current time from the system.
	 *  \return Current time in secs
	 */
	double GetTimeD() const;
	double counter; //!< Member variable "m_Counter"

	LARGE_INTEGER freq;
	bool useHPT;
};
}

#endif // __WATCH_H__
