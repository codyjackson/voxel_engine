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

class Chunk
{
public:
	class Intersected
	{
	public:
		Intersected(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face);

		glm::ivec3 get_indices() const;
		AxiallyAligned::Voxel::Face get_face() const;

	private:
		glm::ivec3 _indices;
		AxiallyAligned::Voxel::Face _face;
	};

	Chunk(const glm::vec3& leftBottomRight, float voxelSideLength);

	static int get_num_of_voxels_per_side();

	Intersection<Intersected> find_nearest_intersection(const Ray& r) const;

	Mesh get_mesh() const;
	Mesh get_voxel_mesh(const glm::ivec3& indices) const;
	glm::mat4 get_model_matrix() const;

	void add_voxel(const glm::ivec3& indices, const Color& color);
	void delete_voxel(const glm::ivec3& indices);

private:
	void generate_mesh();

	class Voxel
	{
	public:
		Voxel(const Chunk& chunk, const glm::ivec3& indices);

		glm::ivec3 get_indices() const;

		glm::vec3 get_model_left_top_front() const;

		Color get_color() const;

		bool is_visible() const;

		bool is_bounded_by_chunk(int a) const;
		bool is_in_chunk() const;
		bool is_top_occluded() const;
		bool is_bottom_occluded() const;
		bool is_front_occluded() const;
		bool is_back_occluded() const;
		bool is_left_occluded() const;
		bool is_right_occluded() const;

		Voxel get_voxel_above() const;
		Voxel get_voxel_below() const;
		Voxel get_voxel_in_front() const;
		Voxel get_voxel_behind() const;
		Voxel get_voxel_to_right() const;
		Voxel get_voxel_to_left() const;

		Mesh generate_mesh() const;

	private:
		const Chunk& _chunk;
		const glm::ivec3 _indices;
	};

	void for_each_voxel(const std::function<void(const Voxel&)>& f) const;

	class Octree
	{
	public:
		class Node
		{
		public:
			Node(const Chunk& chunk, int voxelsPerSide, const glm::ivec3& leftBottomBack);

			Intersection<Intersected> find_nearest_intersection(const Ray& r) const;

		private:
			glm::ivec3 get_indices() const;

			Intersection<Intersected> transform_intersection(const Intersection<AxiallyAligned::Voxel::Intersected>& intersection) const;

			template<typename RETURN_TYPE>
			std::array<RETURN_TYPE, 8> map_children(const std::function<RETURN_TYPE (const Node&)>& f) const
			{
				return std::array<RETURN_TYPE, 8>{
					f(get_left_bottom_back_child()),
					f(get_right_bottom_back_child()),
					f(get_right_bottom_front_child()),
					f(get_left_bottom_front_child()),
					f(get_left_top_back_child()),
					f(get_right_top_back_child()),
					f(get_right_top_front_child()),
					f(get_left_top_front_child())
				};
			}

			bool is_leaf() const;

			AxiallyAligned::Voxel get_bounding_voxel() const;

			Node get_left_bottom_back_child() const;
			Node get_right_bottom_back_child() const;
			Node get_right_bottom_front_child() const;
			Node get_left_bottom_front_child() const;

			Node get_left_top_back_child() const;
			Node get_right_top_back_child() const;
			Node get_right_top_front_child() const;
			Node get_left_top_front_child() const;

			const Chunk& _chunk;
			const int _voxelsPerSide;
			const glm::ivec3 _leftBottomBack;
		};

		Octree(const Chunk& c);

		Intersection<Intersected> find_nearest_intersection(const Ray& r) const;

	private:
		const Node _root;
	};

	enum VoxelsPerSide{VALUE = 16};
	static_assert((VoxelsPerSide::VALUE != 0) && ((VoxelsPerSide::VALUE & (VoxelsPerSide::VALUE - 1)) == 0), "Voxels Per Side must be a power of two so that it behaves well with Octrees.");

	std::array<std::array<std::array<Color, VoxelsPerSide::VALUE>, VoxelsPerSide::VALUE>, VoxelsPerSide::VALUE> _voxels;
	Octree _octree;
	glm::mat4 _modelMatrix;
	Mesh _mesh;
};