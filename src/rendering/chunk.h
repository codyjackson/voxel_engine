#pragma once

#include "../constants/vector.h"
#include "color.h"
#include "mesh.h"

#include "../spatial/axially_aligned/voxel.h"
#include "../spatial/ray.h"

#include "../utility/functional.h"

#include <array>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <limits>
#include <stdexcept>

template<int VOXELS_PER_SIDE>
class Chunk
{
public:
	static_assert(VOXELS_PER_SIDE && !(VOXELS_PER_SIDE & (VOXELS_PER_SIDE - 1)), "VOXELS_PER_SIDE must be a power of 2.");
	Chunk(const glm::vec3 topLeftFront, float voxelSideLength)
		: _modelMatrix(glm::scale(glm::mat4(), glm::vec3(voxelSideLength, voxelSideLength, voxelSideLength)))
	{
		for(int x = 0; x < VOXELS_PER_SIDE; ++x)
			for(int y = 0; y < VOXELS_PER_SIDE; ++y)
				for(int z = 0; z < VOXELS_PER_SIDE; ++z)
					_voxels[x][y][z] =  Color(255, 0, 0, 255);

		_mesh = generate_mesh();
	}

	Mesh get_mesh() const
	{
		return _mesh;
	}

	glm::mat4 get_model_matrix() const
	{
		return _modelMatrix;
	}

	void show_voxel(const glm::uvec3& position, const Color& color){}
	void hide_voxel(const glm::uvec3& position){}

private:
	Mesh generate_mesh() const
	{
		Mesh mesh;
		reduce<Mesh>(mesh, [](Mesh& m, const Voxel& v){
			m.concatenate(v.generate_mesh());
		});
		return mesh;
	}

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

		Mesh generate_mesh() const
		{
			Mesh m;
			if (!is_visible())
				return m;

			if (!is_front_occluded())
				m.push_back(Quad::generate_xy_quad(get_color(), get_model_top_left_front(), Quad::CounterClockWise()));
			if (!is_top_occluded())
				m.push_back(Quad::generate_xz_quad(get_color(), get_model_top_left_front(), Quad::CounterClockWise()));
			if (!is_left_occluded())
				m.push_back(Quad::generate_yz_quad(get_color(), get_model_top_left_front(), Quad::CounterClockWise()));

			if (!is_back_occluded())
				m.push_back(Quad::generate_xy_quad(get_color(), get_model_top_left_front() + Constants::Vec3::forward, Quad::ClockWise()));
			if (!is_bottom_occluded())
				m.push_back(Quad::generate_xz_quad(get_color(), get_model_top_left_front() + Constants::Vec3::down, Quad::ClockWise()));
			if (!is_right_occluded())
				m.push_back(Quad::generate_yz_quad(get_color(), get_model_top_left_front() + Constants::Vec3::right, Quad::ClockWise()));

			return m;
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

	class Octree
	{
	public:
		class Node
		{
		public:
			Node(const Mesh& mesh, int voxelsPerSide, const glm::ivec3& topLeftRront)
				:_mesh(mesh), _voxelsPerSide(voxelsPerSide), _topLeftFront(topLeftRront), _
			{}

			Intersection<Node> find_nearest_intersection(const Ray& r) const
			{
				if (is_leaf())
				{
					const auto intersection = get_bounding_voxel().find_intersection();
					return intersection ? transform_intersection(intersection) : make_intersection();
				}

				if (!(const auto intersection = get_bounding_voxel().find_intersection()))
					return make_intersection<Node>();

				const auto findIntersection = std::bind(&Node::find_nearest_intersection, std::placeholders::_1, std::cref(r));
				const auto intersections = map_children<Intersection<Node>>(findIntersection);

				const auto seed = std::make_pair(std::numeric_limits<float>::infinity, make_intersection<Node>());
				const auto findNearest = [](const std::pair<float, Intersection<Node>>& x, const Intersection<Node>& n){
					const float distance = n->get_distance_from_origin();
					return n && distance < x.first : std::make_pair(distance, n) : x;
				};
				const auto nearest = reduce(begin(intersections), end(intersections), seed, findNearest);
				return nearest.second;
			}

		private:
			static Intersection<Node> transform_intersection(const Intersection<AxiallyAligned::Voxel>& intersection)
			{
				return make_intersection(intersection->get_distance_from_origin(), *this);
			}

			template<typename RETURN_TYPE>
			std::array<RETURN_TYPE, 8> map_children(const std::function<RETURN_TYPE (const Node&)>& f)
			{
				return std::array<RETURN_TYPE, 8>{
					f(get_top_left_front_child()),
					f(get_top_left_back_child()),
					f(get_top_right_front_child()),
					f(get_top_right_back_child()),
					f(get_bottom_left_front_child()),
					f(get_bottom_left_back_child()),
					f(get_bottom_right_front_child()),
					f(get_bottom_right_back_child())
				};
			}

			bool is_leaf() const
			{
				return _voxelsPerSide == 1;
			}

			AxiallyAligned::Voxel get_bounding_voxel() const
			{
				return AxiallyAligned::Voxel(glm::vec3(_topLeftFront), _voxelsPerSide);
			}

			Node get_top_left_front_child() const
			{
				return Node(_mesh, _voxelsPerSide / 2, _topLeftFront);
			}

			Node get_top_left_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(0, 0, -side));
			}

			Node get_top_right_front_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(side, 0, 0));
			}

			Node get_top_right_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(side, 0, -side));
			}

			Node get_bottom_left_front_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(0, -side, 0));
			}

			Node get_bottom_left_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(0, -side, -side));
			}

			Node get_bottom_right_front_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(side, -side, 0));
			}

			Node get_bottom_right_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_mesh, side, _topLeftFront + glm::ivec3(side, -side, -side));
			}

			const Mesh& _mesh;
			int _voxelsPerSide;
			glm::ivec3 _topLeftFront;
		};

		Octree(const Mesh& mesh);
		

	private:
		const Mesh& _mesh;
	};

	std::array<std::array<std::array<Color, VOXELS_PER_SIDE>, VOXELS_PER_SIDE>, VOXELS_PER_SIDE> _voxels;
	glm::mat4 _modelMatrix;
	Mesh _mesh;
};