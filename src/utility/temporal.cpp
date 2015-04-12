#include "temporal.h"

#include <Windows.h>

namespace Temporal
{
	long long get_ms_since_epoch()
	{
		FILETIME fileTime;
		GetSystemTimeAsFileTime(&fileTime);
		return static_cast<long long>(fileTime.dwLowDateTime) + (static_cast<long long>(fileTime.dwHighDateTime) << 32LL);
	}
}