#pragma once

#include "chunk.h"

#include <unordered_map>

class ChunkVault
{
public:
	class Intersected
	{
	public:
		Intersected(const glm::ivec3& chunkIndices, const Chunk::Intersected& chunkIntersected);

		glm::ivec3 get_chunk_indices() const;
		glm::ivec3 get_voxel_indices() const;
		AxiallyAligned::Voxel::Face get_face() const;

	private:
		glm::ivec3 _chunkIndices;
		const Chunk::Intersected _chunkIntersected;
	};

	ChunkVault();

	Intersection<Intersected> find_nearest_intersection(const Ray& r) const;
	glm::ivec3 get_indices_of_voxel_sharing_face_with(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face);
	Mesh get_mesh_of_voxel(const glm::ivec3& indices) const;

	void add_voxel(const glm::ivec3& indices, const Color& c);
	void delete_voxel(const glm::ivec3& indices);

private:
	std::unordered_map<glm::ivec3, Chunk> _leftBottomBackIndicesToChunk;
};