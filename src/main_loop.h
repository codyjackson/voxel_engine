#pragma once

#include "window.h"

#include <functional>

class MainLoop
{
public:
	MainLoop(const std::function<void (Window&)>& onInitialize, const std::function<void (Window&)>& onIteration);

private:
	Window window;
};