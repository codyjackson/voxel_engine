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
	for (int x = -chunkLoadRadiusAroundObserver; x <= chunkLoadRadiusAroundObserver; ++x)
		for (int y = -chunkLoadRadiusAroundObserver; y <= chunkLoadRadiusAroundObserver; ++y)
			for (int z = -chunkLoadRadiusAroundObserver; z <= chunkLoadRadiusAroundObserver; ++z)
				_originToChunk.insert(std::make_pair(glm::ivec3(x, y, z), Chunk(glm::vec3(x, y, z), voxelSideLength)));
	add_voxel(glm::ivec3(0, 0, 0), Color(0xFF, 0xFF, 0xFF, 0xFF));
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

	return Spatial::Utility::get_nearest_intersection(intersections);
}

Mesh ChunkVault::get_mesh_of_voxel(const Intersected& intersected) const
{
	return get_mesh_of_voxel(intersected.get_indices());
}

Mesh ChunkVault::get_mesh_of_voxel(const glm::ivec3& indices) const
{
	const auto chunkOrigin = convert_vault_indices_to_chunk_origin(indices);
	const auto chunkIndices = indices - chunkOrigin;
	return _originToChunk.find(chunkOrigin)->second.get_voxel_mesh(chunkIndices);
}

std::vector<Renderable> ChunkVault::get_renderables() const
{
	std::vector<Renderable> renderables;
	const auto transformToRenderable = [](const std::pair<glm::ivec3, Chunk>& pair){
		return Renderable(pair.second.get_mesh(), pair.second.get_model_matrix());
	};
	std::transform(std::begin(_originToChunk), std::end(_originToChunk), std::back_inserter(renderables), transformToRenderable);
	return renderables;
}

void ChunkVault::add_adjacent_voxel(const Intersected& intersected, const Color& color)
{
	add_adjacent_voxel(intersected.get_indices(), intersected.get_face(), color);
}

void ChunkVault::add_adjacent_voxel(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face, const Color& color)
{
	const auto adjacentVoxelIndices = get_indices_of_adjacent_voxel(indices, face);
	add_voxel(adjacentVoxelIndices, color);
}

void ChunkVault::add_voxel(const glm::ivec3& indices, const Color& color)
{
	const auto chunkOrigin = convert_vault_indices_to_chunk_origin(indices);
	const auto chunkIndices = indices - chunkOrigin;
	_originToChunk.find(chunkOrigin)->second.add_voxel(chunkIndices, color);
}

void ChunkVault::delete_voxel(const Intersected& intersected)
{
	delete_voxel(intersected.get_indices());
}

void ChunkVault::delete_voxel(const glm::ivec3& indices)
{
	const auto chunkOrigin = convert_vault_indices_to_chunk_origin(indices);
	const auto chunkIndices = indices - chunkOrigin;
	_originToChunk.find(chunkOrigin)->second.delete_voxel(chunkIndices);
}

void ChunkVault::update_observers_location(const glm::vec3& oberversLocations)
{
	assert(!"NOT IMPLEMENTED");
}

glm::ivec3 ChunkVault::get_indices_of_adjacent_voxel(const glm::ivec3& indices, AxiallyAligned::Voxel::Face face) const
{
	switch (face)
	{
	case AxiallyAligned::Voxel::Face::TOP:
		return indices + Constants::IVec3::up;
	case AxiallyAligned::Voxel::Face::BOTTOM:
		return indices + Constants::IVec3::down;
	case AxiallyAligned::Voxel::Face::LEFT:
		return indices + Constants::IVec3::left;
	case AxiallyAligned::Voxel::Face::RIGHT:
		return indices + Constants::IVec3::right;
	case AxiallyAligned::Voxel::Face::FRONT:
		return indices + Constants::IVec3::backward;
	default:
		return indices + Constants::IVec3::forward;
	}
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
size_t ChunkVault::IndicesHasher::operator()(const glm::ivec3& indices) const
{
	return std::hash<int>()(indices.x) ^ std::hash<int>()(indices.y) ^ std::hash<int>()(indices.z);
}