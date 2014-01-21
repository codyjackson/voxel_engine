#pragma once

#include "camera.h"
#include "mesh.h"

#include <glm/glm.hpp>

class Renderer
{
public:
	static void render(const Camera& c, const Mesh& mesh, const glm::mat4& modelMatrix);
};