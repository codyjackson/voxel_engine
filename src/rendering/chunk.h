#pragma once

#include "color.h"
#include "mesh.h"

#include <array>
#include <algorithm>
#include <glm\glm.hpp>
#include <gl\GL.h>
#include <iostream>
#include <stdexcept>

template<int SIDE_LENGTH>
class Chunk
{
public:
	Chunk(const glm::vec3 topLeftFront)
		:_topLeftFront(topLeftFront)
	{
		generate_mesh();
	}

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
	void draw_complete_voxel(const glm::vec3& offset)
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

private:
	class Voxel
	{
	public:
		Voxel(const Chunk<SIDE_LENGTH>& chunk, int x, int y, int z)
			: _chunk(chunk), _x(x), _y(y), _z(z)
		{}

		bool is_visible() const
		{
			return _chunk[x][y][z] != Color::INVISIBLE;
		}

		bool is_top_occluded() const
		{
			if(_y == 0)
				return false;
			return Voxel(_x, _y-1, _z).is_visible();
		}

		bool is_bottom_occluded() const
		{
			if(_y == (SIDE_LENGTH-1))
				return false;
			return Voxel(_x, _y+1, _z).is_visible();
		}

		bool is_front_occluded() const
		{
			if(_z == 0)
				return false;
			return Voxel(_x, _y, _z-1).is_visible();
		}

		bool is_back_occluded() const
		{
			if(_z == (SIDE_LENGTH-1))
				return false;
			return Voxel(_x, _y, _z+1).is_visible();
		}

		bool is_left_occluded() const
		{
			if(_x == 0)
				return false;
			return Voxel(_x-1, _y, _z).is_visible();
		}

		bool is_right_occluded() const
		{
			if(_x == (SIDE_LENGTH-1))
				return false;
			return Voxel(_x+1, _y, _z).is_visible();
		}

		int x() const
		{
			return _x;
		}

		int y() const
		{
			return _y;
		}

		int z() const
		{
			return _z;
		}

	private:
		const Chunk<SIDE_LENGTH>& _chunk;
		const int _x;
		const int _y;
		const int _z;
	};

	void for_each(const std::function<void (const Voxel&)>& f) const
	{
		for(int x = 0; x < SIDE_LENGTH; ++x)
			for(int y = 0; y < SIDE_LENGTH; ++y)
				for(int z = 0; z < SIDE_LENGTH; ++z)
					f(Voxel(*this, x, y, z));
	}

	template<typename REDUCTION_TYPE>
	void reduce(REDUCTION_TYPE& seed, const std::function<void (REDUCTION_TYPE&, const Voxel&)>& f) const
	{
		for_each([&seed, &f](const Voxel& v){
			f(seed, v);
		});
	}

	Mesh generate_mesh() const
	{
		int zero = 0;
		reduce<int>(zero, [](int& aggregation, const Voxel& v){
			++aggregation;
		});
		return Mesh();
	}

	std::array<std::array<std::array<Color, SIDE_LENGTH>, SIDE_LENGTH>, SIDE_LENGTH> _voxels;
	glm::vec3 _topLeftFront;
};