#include "main_loop.h"
#include <algorithm>

MainLoop::MainLoop(const std::function<void (Window&)>& onInitialize, const std::function<void (Window&, float timeStepinMs)>& onIteration, float timeStepInSeconds)
try
{
	if (timeStepInSeconds < 0.005f) {
		throw std::runtime_error("200 fps is plenty. Don't be so darn greedy! =P");
	}
	const long long timeStepInMs = static_cast<long long>(timeStepInSeconds*1000.0f);

	Window window;
	window.open();
	onInitialize(window);
	while(window.is_open()) {
		const long long begin = get_ms_since_epoch();

		onIteration(window, timeStepInSeconds);

		window.render();
		window.tick();

		const long long sleep = std::max(timeStepInMs - ((get_ms_since_epoch() - begin) / 1000000), 0LL);
		Sleep(static_cast<int>(sleep));
	}
}
catch (std::exception ex) 
{
	std::cout << ex.what() << std::endl;
	throw;
}

long long MainLoop::get_ms_since_epoch() const
{
	FILETIME fileTime;
	GetSystemTimeAsFileTime(&fileTime);
	return static_cast<long long>(fileTime.dwLowDateTime) + (static_cast<long long>(fileTime.dwHighDateTime) << 32LL);
}