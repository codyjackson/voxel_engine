#pragma once
#include <glm/glm.hpp>

class Voxel
{
	public:
		Voxel();
		Voxel(const glm::vec3 center, const int side); 
	private:
		glm::vec3 center;
		int side;
};