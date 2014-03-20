#include "main_loop.h"

MainLoop::MainLoop(const std::function<void (Window&)>& onInitialize, const std::function<void (Window&, float timeStepinMs)>& onIteration, float timeStepInSeconds)
{
	if (timeStepInSeconds < 0.01f) {
		throw std::runtime_error("100 fps is plenty. Don't be so darn greedy! =P");
	}
	const long long timeStepInMs = static_cast<long long>(timeStepInSeconds*1000.0f);

	Window window;
	window.open();
	onInitialize(window);
	while(window.is_open()) {
		const long long begin = get_ms_since_epoch();
		onIteration(window, timeStepInSeconds);
		window.draw();
		window.tick();
		const long long sleep = timeStepInMs - ((get_ms_since_epoch() - begin) / 1000000);
		Sleep(static_cast<int>(sleep));
	}
}

long long MainLoop::get_ms_since_epoch() const
{
	FILETIME fileTime;
	GetSystemTimeAsFileTime(&fileTime);
	return static_cast<long long>(fileTime.dwLowDateTime) + (static_cast<long long>(fileTime.dwHighDateTime) << 32LL);
}