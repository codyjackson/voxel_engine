#pragma once

#include <glm/glm.hpp>

class Offset : public glm::ivec2
{
public:
	Offset(int x, int y)
		:glm::ivec2(x, y)
	{}
};