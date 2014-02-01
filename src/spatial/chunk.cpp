#include "chunk.h"

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


Chunk::Intersected::Intersected(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face)
	:_indices(indices), _face(face)
{}

glm::ivec3 Chunk::Intersected::get_indices() const
{
	return _indices;
}

AxiallyAligned::Voxel::Face Chunk::Intersected::get_face() const
{
	return _face;
}


Chunk::Chunk(const glm::vec3& leftBottomRight, float voxelSideLength)
	:_octree(*this), _modelMatrix(glm::scale(glm::mat4(), glm::vec3(voxelSideLength, voxelSideLength, voxelSideLength)))
{
	for (int x = 0; x < get_num_of_voxels_per_side(); ++x)
		for (int y = 0; y < get_num_of_voxels_per_side(); ++y)
			for (int z = 0; z < get_num_of_voxels_per_side(); ++z)
				_voxels[x][y][z] = Color(0x0A, 0x91, 0xAB, 255);

	generate_mesh();
}

int Chunk::get_num_of_voxels_per_side() const
{
	return VoxelsPerSide::VALUE;
}

Intersection<Chunk::Intersected> Chunk::find_nearest_intersection(const Ray& r)
{
	const Ray localRay = r.transform_into_new_space(glm::inverse(get_model_matrix()));
	if (auto intersection = _octree.find_nearest_intersection(localRay))
		return intersection;
	return make_intersection<Intersected>();
}

Mesh Chunk::get_mesh() const
{
	return _mesh;
}

Mesh Chunk::get_voxel_mesh(const Intersected& i) const
{
	return Voxel(*this, i.get_indices()).generate_mesh();
}

glm::mat4 Chunk::get_model_matrix() const
{
	return _modelMatrix;
}

void Chunk::show_voxel(const Intersected& i, const Color& color)
{
	const Voxel relativeVoxel(Voxel(*this, i.get_indices()).get_voxel_sharing_face(i.get_face()));
	if (!relativeVoxel.is_in_chunk())
		return;

	const auto ri = relativeVoxel.get_indices();
	_voxels[ri.x][ri.y][ri.z] = color;
	generate_mesh();
}

void Chunk::hide_voxel(const Intersected& indices)
{
	const auto i = indices.get_indices();
	_voxels[i.x][i.y][i.z] = Color(0, 0, 0, 0);
	generate_mesh();
}

void Chunk::generate_mesh()
{
	_mesh.clear();
	for_each_voxel([this](const Voxel& v){
		if (v.is_visible())
			_mesh.concatenate(v.generate_mesh());
	});
}

Chunk::Voxel::Voxel(const Chunk& chunk, const glm::ivec3& indices)
: _chunk(chunk), _indices(indices)
{}

glm::ivec3 Chunk::Voxel::get_indices() const
{
	return _indices;
}

glm::vec3 Chunk::Voxel::get_model_left_top_front() const
{
	return glm::vec3(_indices);
}

Color Chunk::Voxel::get_color() const
{
	return _chunk._voxels[_indices.x][_indices.y][_indices.z];
}

bool Chunk::Voxel::is_visible() const
{
	return get_color().get_alpha() > 0;
}

bool Chunk::Voxel::is_bounded_by_chunk(int a) const
{
	return Numerical::is_between(a, 0, _chunk.get_num_of_voxels_per_side() - 1);
}

bool Chunk::Voxel::is_in_chunk() const
{
	return is_bounded_by_chunk(_indices.x) && is_bounded_by_chunk(_indices.y) && is_bounded_by_chunk(_indices.z);
}

bool Chunk::Voxel::is_top_occluded() const
{
	const auto v = get_voxel_above();
	return v.is_in_chunk() && v.is_visible();
}

bool Chunk::Voxel::is_bottom_occluded() const
{
	const auto v = get_voxel_below();
	return v.is_in_chunk() && v.is_visible();
}

bool Chunk::Voxel::is_front_occluded() const
{
	const auto v = get_voxel_in_front();
	return v.is_in_chunk() && v.is_visible();
}

bool Chunk::Voxel::is_back_occluded() const
{
	const auto v = get_voxel_behind();
	return v.is_in_chunk() && v.is_visible();
}

bool Chunk::Voxel::is_left_occluded() const
{
	const auto v = get_voxel_to_left();
	return v.is_in_chunk() && v.is_visible();
}

bool Chunk::Voxel::is_right_occluded() const
{
	const auto v = get_voxel_to_right();
	return v.is_in_chunk() && v.is_visible();
}

Chunk::Voxel Chunk::Voxel::get_voxel_sharing_face(AxiallyAligned::Voxel::Face face)
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

Chunk::Voxel Chunk::Voxel::get_voxel_above() const
{
	return Voxel(_chunk, _indices + Constants::IVec3::up);
}

Chunk::Voxel Chunk::Voxel::get_voxel_below() const
{
	return Voxel(_chunk, _indices + Constants::IVec3::down);
}

Chunk::Voxel Chunk::Voxel::get_voxel_in_front() const
{
	return Voxel(_chunk, _indices + Constants::IVec3::backward);
}

Chunk::Voxel Chunk::Voxel::get_voxel_behind() const
{
	return Voxel(_chunk, _indices + Constants::IVec3::forward);
}

Chunk::Voxel Chunk::Voxel::get_voxel_to_right() const
{
	return Voxel(_chunk, _indices + Constants::IVec3::right);
}

Chunk::Voxel Chunk::Voxel::get_voxel_to_left() const
{
	return Voxel(_chunk, _indices + Constants::IVec3::left);
}

Mesh Chunk::Voxel::generate_mesh() const
{
	if (!is_visible())
		return Mesh();
	const auto v = AxiallyAligned::Voxel(get_model_left_top_front(), 1.0f);
	return v.generate_mesh(get_color(), !is_front_occluded(), !is_back_occluded(), !is_top_occluded(), !is_bottom_occluded(), !is_left_occluded(), !is_right_occluded());
}

void Chunk::for_each_voxel(const std::function<void(const Voxel&)>& f) const
{
	for (int x = 0; x < get_num_of_voxels_per_side(); ++x)
		for (int y = 0; y < get_num_of_voxels_per_side(); ++y)
			for (int z = 0; z < get_num_of_voxels_per_side(); ++z)
				f(Voxel(*this, glm::ivec3(x, y, z)));
}

Chunk::Octree::Node::Node(const Chunk& chunk, int voxelsPerSide, const glm::ivec3& leftBottomBack)
:_chunk(chunk), _voxelsPerSide(voxelsPerSide), _leftBottomBack(leftBottomBack)
{}

Intersection<Chunk::Intersected> Chunk::Octree::Node::find_nearest_intersection(const Ray& r) const
{
	if (is_leaf())
	{
		if (!Voxel(_chunk, get_indices()).is_visible())
			return make_intersection<Intersected>();
		const auto intersection = get_bounding_voxel().find_intersection(r);
		return intersection ? transform_intersection(intersection) : make_intersection<Intersected>();
	}

	const auto boundingVoxel = get_bounding_voxel();
	if (!boundingVoxel.find_intersection(r) && !boundingVoxel.is_inside(r))
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


glm::ivec3 Chunk::Octree::Node::get_indices() const
{
	return _leftBottomBack;
}

Intersection<Chunk::Intersected> Chunk::Octree::Node::transform_intersection(const Intersection<AxiallyAligned::Voxel::Intersected>& intersection) const
{
	return make_intersection(intersection->get_distance_from_origin(), Intersected(get_indices(), intersection->get_object_of_interest().get_face()));
}

bool Chunk::Octree::Node::is_leaf() const
{
	return _voxelsPerSide == 1;
}

AxiallyAligned::Voxel Chunk::Octree::Node::get_bounding_voxel() const
{
	return AxiallyAligned::Voxel(glm::vec3(_leftBottomBack + Constants::IVec3::up + Constants::IVec3::backward), static_cast<float>(_voxelsPerSide));
}

Chunk::Octree::Node Chunk::Octree::Node::get_left_bottom_back_child() const
{
	return Node(_chunk, _voxelsPerSide / 2, _leftBottomBack);
}

Chunk::Octree::Node Chunk::Octree::Node::get_right_bottom_back_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::right);
}

Chunk::Octree::Node Chunk::Octree::Node::get_right_bottom_front_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::right + Constants::IVec3::backward);
}

Chunk::Octree::Node Chunk::Octree::Node::get_left_bottom_front_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::backward);
}

Chunk::Octree::Node Chunk::Octree::Node::get_left_top_back_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::up);
}

Chunk::Octree::Node Chunk::Octree::Node::get_right_top_back_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::up + Constants::IVec3::right);
}

Chunk::Octree::Node Chunk::Octree::Node::get_right_top_front_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::up + Constants::IVec3::right +  Constants::IVec3::backward);
}

Chunk::Octree::Node Chunk::Octree::Node::get_left_top_front_child() const
{
	const int side = _voxelsPerSide / 2;
	return Node(_chunk, side, _leftBottomBack + Constants::IVec3::up + Constants::IVec3::backward);
}

Chunk::Octree::Octree(const Chunk& c)
	:_root(c, c.get_num_of_voxels_per_side(), glm::ivec3(0.0f, 0.0f, 0.0f))
{}

Intersection<Chunk::Intersected> Chunk::Octree::find_nearest_intersection(const Ray& r) const
{
	return _root.find_nearest_intersection(r);
}