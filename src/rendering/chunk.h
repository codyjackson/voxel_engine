#pragma once

#include "../constants/vector.h"
#include "color.h"
#include "../spatial/intersection.h"
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
static_assert(VOXELS_PER_SIDE && !(VOXELS_PER_SIDE & (VOXELS_PER_SIDE - 1)), "VOXELS_PER_SIDE must be a power of 2. This is necessary for the octree implementation.");
public:
	Chunk(const glm::vec3 topLeftFront, float voxelSideLength)
		:_octree(*this), _modelMatrix(glm::scale(glm::mat4(), glm::vec3(voxelSideLength, voxelSideLength, voxelSideLength)))
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

	class Intersected
	{
	public:
		Intersected(const glm::ivec3& indices)
			:_indices(indices)
		{}

		const glm::ivec3 get_indices() const
		{
			return _indices;
		}

	private:

		glm::ivec3 _indices;
	};

	Intersection<int> find_nearest_intersection(const Ray& r)
	{
		const Ray localRay = r.transform_into_new_space(glm::transpose(get_model_matrix()));
		if (auto intersection = _octree.find_nearest_intersection(localRay))
			return make_intersection<int>(0, 0);
		return make_intersection<int>();
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
			Node(const Chunk<VOXELS_PER_SIDE>& chunk, int voxelsPerSide, const glm::ivec3& topLeftFront)
				:_chunk(chunk), _voxelsPerSide(voxelsPerSide), _topLeftFront(topLeftFront)
			{}

			Intersection<Intersected> find_nearest_intersection(const Ray& r) const
			{
				if (is_leaf())
				{
					const auto intersection = get_bounding_voxel().find_intersection(r);
					return intersection ? transform_intersection(intersection) : make_intersection<Intersected>();
				}

				if (!get_bounding_voxel().find_intersection(r))
					return make_intersection<Intersected>();

				const auto findIntersection = [&r](const Node& n) {
					return n.find_nearest_intersection(r); 
				};
				const auto intersections = map_children<Intersection<Intersected>>(findIntersection);

				const auto findNearest = [](std::pair<float, Intersection<Intersected>> x, const Intersection<Intersected>& n){
					if (!n)
						return x;
					const float distance = n->get_distance_from_origin();
					return n && distance < x.first ? std::make_pair(distance, n) : x;
				};
				const auto seed = std::make_pair(std::numeric_limits<float>::max(), make_intersection<Intersected>());
				const auto nearest = Functional::reduce(std::begin(intersections), std::end(intersections), seed, findNearest);
				return nearest.second;
			}

		private:
			Intersection<Intersected> transform_intersection(const Intersection<AxiallyAligned::Voxel>& intersection) const
			{
				return make_intersection(intersection->get_distance_from_origin(), Intersected(_topLeftFront));
			}

			template<typename RETURN_TYPE>
			std::array<RETURN_TYPE, 8> map_children(const std::function<RETURN_TYPE (const Node&)>& f) const
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
				return AxiallyAligned::Voxel(glm::vec3(_topLeftFront), static_cast<float>(_voxelsPerSide));
			}

			Node get_top_left_front_child() const
			{
				return Node(_chunk, _voxelsPerSide / 2, _topLeftFront);
			}

			Node get_top_left_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(0, 0, -side));
			}

			Node get_top_right_front_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(side, 0, 0));
			}

			Node get_top_right_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(side, 0, -side));
			}

			Node get_bottom_left_front_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(0, -side, 0));
			}

			Node get_bottom_left_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(0, -side, -side));
			}

			Node get_bottom_right_front_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(side, -side, 0));
			}

			Node get_bottom_right_back_child() const
			{
				const int side = _voxelsPerSide / 2;
				return Node(_chunk, side, _topLeftFront + glm::ivec3(side, -side, -side));
			}

			const Chunk<VOXELS_PER_SIDE>& _chunk;
			const int _voxelsPerSide;
			const glm::ivec3 _topLeftFront;
		};

		Octree(const Chunk<VOXELS_PER_SIDE>& c)
			:_root(c, VOXELS_PER_SIDE, glm::ivec3(0.0f, 0.0f, 0.0f))
		{}

		Intersection<Intersected> find_nearest_intersection(const Ray& r) const
		{
			return _root.find_nearest_intersection(r);
		}

	private:
		const Node _root;
	};

	Octree _octree;
	std::array<std::array<std::array<Color, VOXELS_PER_SIDE>, VOXELS_PER_SIDE>, VOXELS_PER_SIDE> _voxels;
	glm::mat4 _modelMatrix;
	Mesh _mesh;
};