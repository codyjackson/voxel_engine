#include "chunk_vault.h"

#include "utility.h"

ChunkVault::Intersected::Intersected(const glm::ivec3& chunkOrigin, const Chunk::Intersected& chunkIntersected)
:_indices(chunkOrigin + chunkIntersected.get_indices()), _face(chunkIntersected.get_face())
{}

glm::ivec3 ChunkVault::Intersected::get_indices() const
{
	return _indices;
}

AxiallyAligned::Voxel::Face ChunkVault::Intersected::get_face() const
{
	return _face;
}

ChunkVault::ChunkVault(float voxelSideLength, const glm::vec3& originLocationInWorld, const glm::vec3& observersLocation, int chunkLoadRadiusAroundObserver)
{
}

Intersection<ChunkVault::Intersected> ChunkVault::find_nearest_intersection(const Ray& r) const
{
	const auto getIntersection = [&r](const std::pair<glm::ivec3, Chunk>& originChunkPair) {
		const auto intersection = originChunkPair.second.find_nearest_intersection(r);
		if (!intersection)
			return make_intersection<Intersected>();
		return make_intersection(intersection->get_distance_from_origin(), Intersected(originChunkPair.first, intersection->get_object_of_interest()));
	};
	std::vector<Intersection<Intersected>> intersections;
	std::transform(_originToChunk.begin(), _originToChunk.end(), std::back_inserter(intersections), getIntersection);

	return Spatial::Utility::get_nearest_intersection(std::begin(intersections), std::end(intersections));
}

glm::ivec3 ChunkVault::get_indices_of_voxel_sharing_face_with(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face)
{
	return glm::ivec3();
}

Mesh ChunkVault::get_mesh_of_voxel(const glm::ivec3& indices) const
{
	return Mesh();
}

void ChunkVault::add_voxel(const glm::ivec3& indices, const Color& c)
{
	assert(!"NOT IMPLEMENTED");
}

void ChunkVault::delete_voxel(const glm::ivec3& indices)
{
	assert(!"NOT IMPLEMENTED");
}

void ChunkVault::update_observers_location(const glm::vec3& oberversLocations)
{
	assert(!"NOT IMPLEMENTED");
}

glm::vec3 ChunkVault::convert_world_space_to_vault_space(const glm::vec3& v) const
{
	return v / _voxelSideLength;
}

glm::ivec3 ChunkVault::convert_vault_indices_to_chunk_origin(const glm::ivec3& indices) const
{
	return indices / Chunk::get_num_of_voxels_per_side();
}

glm::ivec3 ChunkVault::convert_world_space_to_chunk_origin(const glm::vec3& worldLocation) const
{
	const glm::ivec3 vaultIndices(convert_world_space_to_vault_space(worldLocation));
	return convert_vault_indices_to_chunk_origin(vaultIndices);

}