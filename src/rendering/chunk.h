#pragma once

#include "../constants/vector.h"
#include "color.h"
#include "mesh.h"

#include <array>
#include <algorithm>
#include <glm\glm.hpp>
#include <gl\GL.h>
#include <iostream>
#include <stdexcept>

template<int VOXELS_PER_SIDE>
class Chunk
{
public:
	Chunk(const glm::vec3 topLeftFront, float sideLength)
		:_topLeftFront(topLeftFront), _scalingFactor(sideLength/static_cast<float>(VOXELS_PER_SIDE))
	{
		for(int x = 0; x < VOXELS_PER_SIDE; ++x)
			for(int y = 0; y < VOXELS_PER_SIDE; ++y)
				for(int z = 0; z < VOXELS_PER_SIDE; ++z)
					_voxels[x][y][z] =  Color(255, 0, 0, 255);

		_mesh = generate_mesh();
	}

	void draw(const glm::mat4& projectionView)
	{
		glPushMatrix();
		glTranslatef(_topLeftFront.x, _topLeftFront.y, _topLeftFront.z);
		glScalef(_scalingFactor, _scalingFactor, _scalingFactor);
		

		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
			glBegin(GL_QUADS);
			_mesh.draw_with_color();
			glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1);
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		_mesh.draw_without_color();
		glEnd();

		glPopMatrix();
	}

	void show_voxel(const glm::uvec3& position, const Color& color){}
	void hide_voxel(const glm::uvec3& position){}

private:
	class Voxel
	{
	public:
		Voxel(const Chunk<VOXELS_PER_SIDE>& chunk, int x, int y, int z)
			: _chunk(chunk), _x(x), _y(y), _z(z)
		{}

		glm::vec3 get_model_top_left_front() const
		{
			return glm::vec3(_x, -_y, -_z);
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
			if(_y == (VOXELS_PER_SIDE-1))
				return false;
			return Voxel(_chunk, _x, _y+1, _z).is_visible();
		}

		bool is_front_occluded() const
		{
			if (_z == 0)
				return false;
			return Voxel(_chunk, _x, _y, _z-1).is_visible();
		}

		bool is_back_occluded() const
		{
			if (_z == (VOXELS_PER_SIDE - 1))
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
			if(_x == (VOXELS_PER_SIDE-1))
				return false;
			return Voxel(_chunk, _x+1, _y, _z).is_visible();
		}

		Mesh generate_mesh() const
		{
			Mesh m;
			if(!is_visible())
				return m;

			if(!is_front_occluded())
				m.push_back(Quad::generate_xy_quad(get_color(), get_model_top_left_front(), Quad::CounterClockWise()));
			if(!is_top_occluded())
				m.push_back(Quad::generate_xz_quad(get_color(), get_model_top_left_front(), Quad::CounterClockWise()));
			if(!is_left_occluded())
				m.push_back(Quad::generate_yz_quad(get_color(), get_model_top_left_front(), Quad::CounterClockWise()));

			if(!is_back_occluded())
				m.push_back(Quad::generate_xy_quad(get_color(), get_model_top_left_front() + Constants::Vec3::forward, Quad::ClockWise()));
			if(!is_bottom_occluded())
				m.push_back(Quad::generate_xz_quad(get_color(), get_model_top_left_front() + Constants::Vec3::down, Quad::ClockWise()));
			if(!is_right_occluded())
				m.push_back(Quad::generate_yz_quad(get_color(), get_model_top_left_front() + Constants::Vec3::right, Quad::ClockWise()));

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
		const Chunk<VOXELS_PER_SIDE>& _chunk;
		const int _x;
		const int _y;
		const int _z;
	};

	void for_each(const std::function<void (const Voxel&)>& f) const
	{
		for(int x = 0; x < VOXELS_PER_SIDE; ++x)
			for(int y = 0; y < VOXELS_PER_SIDE; ++y)
				for(int z = 0; z < VOXELS_PER_SIDE; ++z)
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

	std::array<std::array<std::array<Color, VOXELS_PER_SIDE>, VOXELS_PER_SIDE>, VOXELS_PER_SIDE> _voxels;
	glm::vec3 _topLeftFront;
	float _scalingFactor;
	Mesh _mesh;
};