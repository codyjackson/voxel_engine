#pragma once

#include "window.h"

#include <functional>

class MainLoop
{
public:
	MainLoop(const std::function<void (Window&)>& onInitialize, const std::function<void (Window&, float timeStep)>& onIteration, float timeStepInSeconds);

private:
	long long get_ms_since_epoch() const;
};