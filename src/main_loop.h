#pragma once

#include "window.h"
#include <algorithm>
#include <functional>

template <typename GAME>
class MainLoop
{
public:
	MainLoop(float timeStepInSeconds)
	{
		if (timeStepInSeconds < 0.005f) {
			throw std::runtime_error("200 fps is plenty. Don't be so darn greedy! =P");
		}
		const long long timeStepInMs = static_cast<long long>(timeStepInSeconds*1000.0f);

		Window window;
		window.open();

		GAME game(window);
		while (window.is_open()) {
			const long long begin = get_ms_since_epoch();

			game.onIteration(window, timeStepInSeconds);

			window.swap_buffers();
			window.tick();

			const long long sleep = std::max(timeStepInMs - ((get_ms_since_epoch() - begin) / 1000000), 0LL);
			Sleep(static_cast<int>(sleep));
		}
	}

private:
	long long get_ms_since_epoch() const
	{
		FILETIME fileTime;
		GetSystemTimeAsFileTime(&fileTime);
		return static_cast<long long>(fileTime.dwLowDateTime) + (static_cast<long long>(fileTime.dwHighDateTime) << 32LL);
	}
};