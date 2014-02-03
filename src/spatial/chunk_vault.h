#pragma once

#include "chunk.h"

#include <unordered_map>

class ChunkVault
{
public:
	class Intersected
	{
	public:
		Intersected(const glm::ivec3& chunkOrigin, const Chunk::Intersected& chunkIntersected);

		glm::ivec3 get_indices() const;
		AxiallyAligned::Voxel::Face get_face() const;

	private:
		glm::ivec3 _indices;
		AxiallyAligned::Voxel::Face _face;
	};

	ChunkVault(float voxelSideLength, const glm::vec3& originLocationInWorld, const glm::vec3& observersLocation, int chunkLoadRadiusAroundObserver);

	Intersection<Intersected> find_nearest_intersection(const Ray& r) const;
	glm::ivec3 get_indices_of_voxel_sharing_face_with(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face);
	Mesh get_mesh_of_voxel(const glm::ivec3& indices) const;

	void add_voxel(const glm::ivec3& indices, const Color& c);
	void delete_voxel(const glm::ivec3& indices);
	void update_observers_location(const glm::vec3& oberversLocations);

private:
	glm::vec3 convert_world_space_to_vault_space(const glm::vec3& v) const;
	glm::ivec3 convert_vault_indices_to_chunk_origin(const glm::ivec3& indices) const;
	glm::ivec3 convert_world_space_to_chunk_origin(const glm::vec3& worldLocation) const;

	struct IndicesHasher { size_t operator()(const glm::ivec3& indices) const; };

	float _voxelSideLength;
	const glm::vec3 _originLocationInWorld;
	const glm::vec3 _observersLocation;
	int _chunkLoadRadius;
	std::unordered_map<glm::ivec3, Chunk, IndicesHasher> _originToChunk;
};