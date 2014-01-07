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
		for(int x = 0; x < SIDE_LENGTH; ++x)
			for(int y = 0; y < SIDE_LENGTH; ++y)
				for(int z = 0; z < SIDE_LENGTH; ++z)
					_voxels[x][y][z] = y == 0 ? Color(255, 0, 0, 255) : Color::INVISIBLE;

		_mesh = generate_mesh();
	}

	void draw(const glm::mat4& projectionView)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(8);
			glColor3f(0,0,0);
			draw_mesh();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
			glColor3f(1,1,1);
			draw_mesh();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void show_voxel(const glm::uvec3& position, const Color& color){}
	void hide_voxel(const glm::uvec3& position){}

private:
	void draw_mesh() const
	{
		glPushMatrix();
		glScalef(0.05f, 0.05f, 0.05f);

		glBegin(GL_QUADS);
		_mesh.draw();
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

		glm::vec3 get_top_left() const
		{
			return glm::vec3(_x, _y, _z) + _chunk._topLeftFront;
		}

		Color get_color() const
		{
			return _chunk._voxels[_x][_y][_z];
		}

		bool is_visible() const
		{
			return get_color().get_alpha() > 0;
		}

		bool is_top_occluded() const
		{
			if(_y == 0)
				return false;
			return Voxel(_chunk, _x, _y-1, _z).is_visible();
		}

		bool is_bottom_occluded() const
		{
			if(_y == (SIDE_LENGTH-1))
				return false;
			return Voxel(_chunk, _x, _y+1, _z).is_visible();
		}

		bool is_front_occluded() const
		{
			if(_z == 0)
				return false;
			return Voxel(_chunk, _x, _y, _z-1).is_visible();
		}

		bool is_back_occluded() const
		{
			if(_z == (SIDE_LENGTH-1))
				return false;
			return Voxel(_chunk, _x, _y, _z+1).is_visible();
		}

		bool is_left_occluded() const
		{
			if(_x == 0)
				return false;
			return Voxel(_chunk, _x-1, _y, _z).is_visible();
		}

		bool is_right_occluded() const
		{
			if(_x == (SIDE_LENGTH-1))
				return false;
			return Voxel(_chunk, _x+1, _y, _z).is_visible();
		}

		Mesh generate_mesh() const
		{
			Mesh m;
			if(!is_visible())
				return m;

			if(!is_front_occluded())
				m.push_back(Quad::generate_xy_quad(get_color(), get_top_left(), Quad::ClockWise()));
			if(!is_top_occluded())
				m.push_back(Quad::generate_xz_quad(get_color(), get_top_left(), Quad::ClockWise()));
			if(!is_left_occluded())
				m.push_back(Quad::generate_yz_quad(get_color(), get_top_left(), Quad::ClockWise()));

			if(!is_back_occluded())
				m.push_back(Quad::generate_xy_quad(get_color(), get_top_left()+glm::vec3(0.0f, 0.0f, 1.0f), Quad::CounterClockWise()));
			if(!is_bottom_occluded())
				m.push_back(Quad::generate_xz_quad(get_color(), get_top_left()+glm::vec3(0.0f, 1.0f, 0.0f), Quad::CounterClockWise()));
			if(!is_right_occluded())
				m.push_back(Quad::generate_yz_quad(get_color(), get_top_left()+glm::vec3(1.0f, 0.0f, 0.0f), Quad::CounterClockWise()));

			return m;
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
		Mesh mesh;
		reduce<Mesh>(mesh, [](Mesh& m, const Voxel& v){
			m.concatenate(v.generate_mesh());
		});
		return mesh;
	}

	std::array<std::array<std::array<Color, SIDE_LENGTH>, SIDE_LENGTH>, SIDE_LENGTH> _voxels;
	glm::vec3 _topLeftFront;
	Mesh _mesh;
};