#pragma once

#include "../browser/jsvalue.h"
#include "../spatial/chunk_vault.h"
#include "../rendering/color.h"

class Editor : boost::noncopyable
{
public:
	Editor(ChunkVault& chunkVault, const Camera& camera);

	JSValue create_ui_api();
	void add_voxel_using_line_of_site(const Color& c);
	void remove_voxel_using_line_of_site();

private:
	void add_voxel_using_line_of_site(const JSValue::Array& args);

	ChunkVault& _chunkVault;
	const Camera& _camera;
};