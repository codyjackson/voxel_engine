#pragma once

#include "../spatial/chunk_vault.h"
#include "../rendering/color.h"

class Editor : boost::noncopyable
{
public:
	Editor(ChunkVault& chunkVault, const Camera& camera)
		:_chunkVault(chunkVault), _camera(camera)
	{}

	void add_voxel_using_line_of_site(const Color& c)
	{
		_camera.
	}

private:
	ChunkVault& _chunkVault;
	const Camera& _camera;
};