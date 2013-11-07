#pragma once

#include "color.h"

#include <array>
#include <glm\glm.hpp>
#include <gl\GL.h>

template<int SIDE_LENGTH>
class Chunk
{
public:
	Chunk(const glm::vec3 topLeftFront)
		:_topLeftFront(topLeftFront)
	{}

	void draw(const glm::mat4& projectionView)
	{
		for(int x = 1; x < SIDE_LENGTH; ++x)
			for(int y = 1; y < SIDE_LENGTH; ++y)
				for(int z = 1; z < SIDE_LENGTH; ++z)
					draw_complete_voxel(glm::vec3(x,y,z)+_topLeftFront);
	}

	void show_voxel(const glm::uvec3& position, const Color& color){}
	void hide_voxel(const glm::uvec3& position){}

private:
	void draw_complete_voxel(const glm::vec3& offset) const
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(8);
			glColor3f(0,0,0);
			draw_voxel(offset);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
			glColor3f(1,1,1);
			draw_voxel(offset);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void draw_voxel(const glm::vec3& offset) const
	{
		float l_length = 0.5f;
		float l_height = 0.5f;
		float l_width = 0.5f;

		glPushMatrix();
		glScalef(0.05f, 0.05f, 0.05f);
		glTranslatef(offset.x, offset.y, offset.z);

		glBegin(GL_QUADS);
			glVertex3f(l_length, -l_height, -l_width);
			glVertex3f(-l_length, -l_height, -l_width);
			glVertex3f(-l_length, l_height, -l_width);
			glVertex3f(l_length, l_height, -l_width);

			glVertex3f(-l_length, -l_height, l_width);
			glVertex3f(l_length, -l_height, l_width);
			glVertex3f(l_length, l_height, l_width);
			glVertex3f(-l_length, l_height, l_width);

			glVertex3f(l_length, -l_height, l_width);
			glVertex3f(l_length, -l_height, -l_width);
			glVertex3f(l_length, l_height, -l_width);
			glVertex3f(l_length, l_height, l_width);

			glVertex3f(-l_length, -l_height, -l_width);
			glVertex3f(-l_length, -l_height, l_width);
			glVertex3f(-l_length, l_height, l_width);
			glVertex3f(-l_length, l_height, -l_width);

			glVertex3f(-l_length, -l_height, -l_width);
			glVertex3f(l_length, -l_height, -l_width);
			glVertex3f(l_length, -l_height, l_width);
			glVertex3f(-l_length, -l_height, l_width);

			glVertex3f(l_length, l_height, -l_width);
			glVertex3f(-l_length, l_height, -l_width);
			glVertex3f(-l_length, l_height, l_width);
			glVertex3f(l_length, l_height, l_width);
		glEnd();

		glPopMatrix();
	}

	std::array<std::array<std::array<Color, SIDE_LENGTH>, SIDE_LENGTH>, SIDE_LENGTH> _voxels;
	glm::vec3 _topLeftFront;
};