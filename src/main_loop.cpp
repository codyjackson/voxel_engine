#include "main_loop.h"

MainLoop::MainLoop(const std::function<void (Window&)>& onInitialize, const std::function<void (Window&)>& onIteration)
{
	onInitialize(window);
	window.open();
	while(window.is_open())
	{
		onIteration(window);
		window.draw();
		window.tick();
	}
}