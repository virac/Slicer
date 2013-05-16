#include "Watch.h"
#include "Config.h"

#ifdef SYSTEM_LINUX
#include <unistd.h>
#include <sys/time.h>
#elif defined SYSTEM_WINDOWS
#include <windows.h>
#endif
namespace Kernal {
Watch::Watch() {
#ifdef SYSTEM_WINDOWS
	useHPT = QueryPerformanceFrequency(&freq);
#else
	useHPT = false;
#endif
	Reset();
}

double Watch::GetSplit() const {
	return GetTimeD() - counter;
}

void Watch::Reset() {
	counter = GetTimeD();
}

Kernal::uint32 Watch::GetTime() {
#ifdef SYSTEM_LINUX
	timeval cTime = {0,0};
	gettimeofday(&cTime, NULL);

	return cTime.tv_sec + cTime.tv_usec / MSEC_TO_USEC;
#elif defined SYSTEM_WINDOWS
	static LARGE_INTEGER f;
	static bool use_high = QueryPerformanceFrequency(&f);
	if (use_high) {
		LARGE_INTEGER cTime;
		QueryPerformanceCounter(&cTime);

		return (cTime.QuadPart * SEC_TO_MSEC) / (f.QuadPart);
	} else {
		return GetTickCount();
	}
#endif
}

double Watch::GetTimeD() const {
#ifdef SYSTEM_LINUX
	timeval cTime = {0,0};
	gettimeofday(&cTime, NULL);

	return cTime.tv_sec + cTime.tv_usec / SEC_TO_USEC;
#elif defined SYSTEM_WINDOWS
	if (useHPT) {
		LARGE_INTEGER cTime;
		QueryPerformanceCounter(&cTime);

		return static_cast<double>(cTime.QuadPart)
				/ static_cast<double>(freq.QuadPart);
	} else {
		return GetTickCount() / SEC_TO_MSEC;
	}
#endif
}

void Watch::SleepSec(float time) {
#ifdef SYSTEM_LINUX
	usleep(static_cast<unsigned long>(time * SEC_TO_USEC));
#elif defined SYSTEM_WINDOWS
	Sleep(static_cast<DWORD>(time * SEC_TO_MSEC));
#endif
}
}

