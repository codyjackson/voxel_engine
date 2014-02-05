#include "chunk_vault.h"

#include "../rendering/renderer.h"
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
				_originToChunk.insert(std::make_pair(glm::ivec3(x, y, z)*Chunk::get_num_of_voxels_per_side(), std::make_shared<Chunk>(glm::vec3(x, y, z) * static_cast<float>(Chunk::get_num_of_voxels_per_side()) * voxelSideLength, voxelSideLength)));
	add_voxel(glm::ivec3(0, 0, 0), Color(0xFF, 0xFF, 0xFF, 0xFF));
}

Intersection<ChunkVault::Intersected> ChunkVault::find_nearest_intersection(const Ray& r) const
{
	const auto getIntersection = [&r](const std::pair<glm::ivec3, std::shared_ptr<Chunk>>& originChunkPair) {
		const auto intersection = originChunkPair.second->find_nearest_intersection(r);
		if (!intersection)
			return make_intersection<Intersected>();
		return make_intersection(intersection->get_distance_from_origin(), Intersected(originChunkPair.first, intersection->get_object_of_interest()));
	};
	std::vector<Intersection<Intersected>> intersections;
	std::transform(_originToChunk.begin(), _originToChunk.end(), std::back_inserter(intersections), getIntersection);

	return Spatial::Utility::get_nearest_intersection(intersections);
}

glm::mat4 ChunkVault::get_voxel_model_matrix(const Intersected& intersected) const
{
	return get_voxel_model_matrix(intersected.get_indices());
}

glm::mat4 ChunkVault::get_voxel_model_matrix(const glm::ivec3& indices) const
{
	const auto chunkOrigin = convert_vault_indices_to_chunk_origin(indices);
	return _originToChunk.find(chunkOrigin)->second->get_model_matrix();
}

Mesh ChunkVault::get_mesh_of_voxel(const Intersected& intersected) const
{
	return get_mesh_of_voxel(intersected.get_indices());
}

Mesh ChunkVault::get_mesh_of_voxel(const glm::ivec3& indices) const
{
	const auto chunkOrigin = convert_vault_indices_to_chunk_origin(indices);
	const auto chunkIndices = indices - chunkOrigin;
	return _originToChunk.find(chunkOrigin)->second->get_voxel_mesh(chunkIndices);
}

void ChunkVault::render(const Camera& camera) const
{
	const auto render = [&camera](const std::pair<glm::ivec3, std::shared_ptr<Chunk>>& pair){
		if (pair.second->get_mesh().size() <= 0)
			return;
		Renderer::render(camera, pair.second->get_model_matrix(), pair.second->get_mesh());
		Renderer::render_wireframe(camera, pair.second->get_model_matrix(), Color(0x0C, 0x22, 0x33, 255), pair.second->get_mesh());
	};
	std::for_each(std::begin(_originToChunk), std::end(_originToChunk), render);
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
	_originToChunk.find(chunkOrigin)->second->add_voxel(chunkIndices, color);
}

void ChunkVault::delete_voxel(const Intersected& intersected)
{
	delete_voxel(intersected.get_indices());
}

void ChunkVault::delete_voxel(const glm::ivec3& indices)
{
	const auto chunkOrigin = convert_vault_indices_to_chunk_origin(indices);
	const auto chunkIndices = indices - chunkOrigin;
	_originToChunk.find(chunkOrigin)->second->delete_voxel(chunkIndices);
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
	int x = (indices.x >= 0 ? indices.x : indices.x - Chunk::get_num_of_voxels_per_side());
	int y = (indices.y >= 0 ? indices.y : indices.y - Chunk::get_num_of_voxels_per_side());
	int z = (indices.z >= 0 ? indices.z : indices.z - Chunk::get_num_of_voxels_per_side());
	return (glm::ivec3(x, y, z) / Chunk::get_num_of_voxels_per_side()) * Chunk::get_num_of_voxels_per_side();
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