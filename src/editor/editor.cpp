#include "editor.h"

Editor::Editor(ChunkVault& chunkVault, const Camera& camera)
:_chunkVault(chunkVault), _camera(camera)
{}

JSValue Editor::create_ui_api()
{
	JSValue::Object root;	
	root["removeVoxel"] = JSValue::wrap_void_function(std::bind(&Editor::remove_voxel_using_line_of_site, this));
	root["addVoxel"] = [this](const JSValue::Array& args){
		if ((args.size() != 1) || !args[0].is_object()) {
			throw std::runtime_error("Expected a color to be passed.");
		}
		add_voxel_using_line_of_site(args);
		return JSValue::Array();
	};

	return root;
}

void Editor::add_voxel_using_line_of_site(const Color& c)
{
	Ray ray(_camera.get_ray_projected_through_center());
	if (const auto intersection = _chunkVault.find_nearest_intersection(ray)) {
		_chunkVault.add_adjacent_voxel(intersection->get_object_of_interest(), c);
	}
}

void Editor::remove_voxel_using_line_of_site()
{
	Ray ray(_camera.get_ray_projected_through_center());
	if (const auto intersection = _chunkVault.find_nearest_intersection(ray)) {
		_chunkVault.delete_voxel(intersection->get_object_of_interest());
	}
}


void Editor::add_voxel_using_line_of_site(const JSValue::Array& args)
{
	JSValue::Object color = args[0];
	unsigned char r = static_cast<unsigned char>(static_cast<double>(color["r"]));
	unsigned char g = static_cast<unsigned char>(static_cast<double>(color["g"]));
	unsigned char b = static_cast<unsigned char>(static_cast<double>(color["b"]));

	add_voxel_using_line_of_site(Color(r, g, b));
}