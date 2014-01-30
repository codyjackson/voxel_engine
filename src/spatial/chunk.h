#pragma once

#include "../constants/vector.h"

#include "../rendering/color.h"
#include "../rendering/mesh.h"

#include "../spatial/axially_aligned/voxel.h"
#include "../spatial/intersection.h"
#include "../spatial/ray.h"

#include "../utility/functional.h"
#include "../utility/numerical.h"


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
	class Intersected
	{
	public:
		Intersected(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face)
			:_indices(indices), _face(face)
		{}

		const glm::ivec3 get_indices() const
		{
			return _indices;
		}

		AxiallyAligned::Voxel::Face get_face() const
		{
			return _face;
		}

	private:

		glm::ivec3 _indices;
		AxiallyAligned::Voxel::Face _face;
	};

	Chunk(const glm::vec3& topLeftFront, float voxelSideLength)
		:_octree(*this), _modelMatrix(glm::scale(glm::mat4(), glm::vec3(voxelSideLength, voxelSideLength, voxelSideLength)))
	{
		for(int x = 0; x < VOXELS_PER_SIDE; ++x)
			for(int y = 0; y < VOXELS_PER_SIDE; ++y)
				for(int z = 0; z < VOXELS_PER_SIDE; ++z)
					_voxels[x][y][z] = Color(0x0A, 0x91, 0xAB, 255);

		generate_mesh();
	}

	Intersection<Intersected> find_nearest_intersection(const Ray& r)
	{
		const Ray localRay = r.transform_into_new_space(glm::inverse(get_model_matrix()));
		if (auto intersection = _octree.find_nearest_intersection(localRay))
			return intersection;
		return make_intersection<Intersected>();
	}

	Mesh get_mesh() const
	{
		return _mesh;
	}

	Mesh get_voxel_mesh(const Intersected& i) const
	{
		const auto indices = i.get_indices();
		return Voxel(*this, indices.x, indices.y, indices.z).generate_mesh();
	}

	glm::mat4 get_model_matrix() const
	{
		return _modelMatrix;
	}

	void show_voxel(const Intersected& i, const Color& color)
	{
		const auto indices = i.get_indices();
		const Voxel relativeVoxel(Voxel(*this, indices.x, indices.y, indices.z).get_relative_voxel(i.get_face()));
		if (!relativeVoxel.is_in_chunk())
			return;

		const auto ri = relativeVoxel.get_indices();
		_voxels[ri.x][ri.y][ri.z] = color;
		generate_mesh();
	}

	void hide_voxel(const Intersected& indices)
	{
		const auto i = indices.get_indices();
		_voxels[i.x][i.y][i.z] = Color(0, 0, 0, 0);
		generate_mesh();
	}

private:
	void generate_mesh()
	{
		_mesh.clear();
		for_each_voxel([this](const Voxel& v){
			if (v.is_visible())
				_mesh.concatenate(v.generate_mesh());
		});
	}

	class Voxel
	{
	public:
		Voxel(const Chunk<VOXELS_PER_SIDE>& chunk, int x, int y, int z)
			: _chunk(chunk), _x(x), _y(y), _z(z)
		{}

		glm::ivec3 get_indices() const
		{
			return glm::ivec3(_x, _y, _z);
		}

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

		bool is_bounded_by_chunk(int a) const
		{
			return Numerical::is_between(a, 0, VOXELS_PER_SIDE - 1);
		}

		bool is_in_chunk() const
		{
			return is_bounded_by_chunk(_x) && is_bounded_by_chunk(_y) && is_bounded_by_chunk(_z);
		}

		bool is_top_occluded() const
		{
			const auto v = get_voxel_above();
			return v.is_in_chunk() && v.is_visible();
		}

		bool is_bottom_occluded() const
		{
			const auto v = get_voxel_below();
			return v.is_in_chunk() && v.is_visible();
		}

		bool is_front_occluded() const
		{
			const auto v = get_voxel_in_front();
			return v.is_in_chunk() && v.is_visible();
		}

		bool is_back_occluded() const
		{
			const auto v = get_voxel_behind();
			return v.is_in_chunk() && v.is_visible();
		}

		bool is_left_occluded() const
		{
			const auto v = get_voxel_to_left();
			return v.is_in_chunk() && v.is_visible();
		}

		bool is_right_occluded() const
		{
			const auto v = get_voxel_to_right();
			return v.is_in_chunk() && v.is_visible();
		}

		Voxel get_relative_voxel(AxiallyAligned::Voxel::Face face)
		{
			switch (face)
			{
			case AxiallyAligned::Voxel::Face::TOP:
				return get_voxel_above();
			case AxiallyAligned::Voxel::Face::BOTTOM:
				return get_voxel_below();
			case AxiallyAligned::Voxel::Face::LEFT:
				return get_voxel_to_left();
			case AxiallyAligned::Voxel::Face::RIGHT:
				return get_voxel_to_right();
			case AxiallyAligned::Voxel::Face::FRONT:
				return get_voxel_in_front();
			default:
				return get_voxel_behind();
			}
		}

		Voxel get_voxel_above() const
		{
			return Voxel(_chunk, _x, _y - 1, _z);
		}

		Voxel get_voxel_below() const
		{
			return Voxel(_chunk, _x, _y + 1, _z);
		}

		Voxel get_voxel_in_front() const
		{
			return Voxel(_chunk, _x, _y, _z - 1);
		}

		Voxel get_voxel_behind() const
		{
			return Voxel(_chunk, _x, _y, _z + 1);
		}

		Voxel get_voxel_to_right() const
		{
			return Voxel(_chunk, _x + 1, _y, _z);
		}

		Voxel get_voxel_to_left() const
		{
			return Voxel(_chunk, _x - 1, _y, _z);
		}

		Mesh generate_mesh() const
		{
			if (!is_visible())
				return Mesh();
			const auto v = AxiallyAligned::Voxel(get_model_top_left_front(), 1.0f);
			return v.generate_mesh(get_color(), !is_front_occluded(), !is_back_occluded(), !is_top_occluded(), !is_bottom_occluded(), !is_left_occluded(), !is_right_occluded());
		}

	private:
		const Chunk<VOXELS_PER_SIDE>& _chunk;
		const int _x;
		const int _y;
		const int _z;
	};

	void for_each_voxel(const std::function<void (const Voxel&)>& f) const
	{
		for(int x = 0; x < VOXELS_PER_SIDE; ++x)
			for(int y = 0; y < VOXELS_PER_SIDE; ++y)
				for(int z = 0; z < VOXELS_PER_SIDE; ++z)
					f(Voxel(*this, x, y, z));
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
					const auto i(get_indices());
					if (!Voxel(_chunk, i.x, i.y, i.z).is_visible())
						return make_intersection<Intersected>();
					const auto intersection = get_bounding_voxel().find_intersection(r);
					return intersection ? transform_intersection(intersection) : make_intersection<Intersected>();
				}

				const auto boundingVoxel = get_bounding_voxel();
				if (!get_bounding_voxel().find_intersection(r) && !boundingVoxel.is_inside(r))
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
			glm::ivec3 get_indices() const
			{
				return glm::ivec3(_topLeftFront.x, -1.0f*_topLeftFront.y, -1.0f*_topLeftFront.z);
			}

			Intersection<Intersected> transform_intersection(const Intersection<AxiallyAligned::Voxel::Intersected>& intersection) const
			{
				return make_intersection(intersection->get_distance_from_origin(), Intersected(get_indices(), intersection->get_object_of_interest().get_face()));
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