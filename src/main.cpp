#include <Windows.h>
#include "main_loop.h"

#include "editor/editor.h"
#include "player.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "spatial/chunk_vault.h"
#include "ui.h"
#include "units.h"

#include <glm/glm.hpp>
#include <memory>


#include "rendering/gl_rendering/vertex_array_object.h"
#include "rendering/gl_rendering/vertex_buffer_object.h"
#include "rendering/gl_rendering/program.h"
#include "rendering/gl_rendering/buffer.h"
#include "rendering/gl_rendering/error.h"

int main(int argc, char* argv[])
{
	Player player;
	player.move_forward(-meters(2.0));
	//UI ui;

	Camera camera(Transform::make_transform(glm::vec3(), Orientation(), player.get_transform()));

	ChunkVault chunkVault(meters(2.0f / 13.0f), glm::vec3(0, 0, 0));
	RectSize resolution(1024, 768);
	Editor editor(chunkVault, camera);

	std::shared_ptr<VertexArrayObject> vao;
	std::shared_ptr<VertexBufferObject> buffer;
	std::shared_ptr<Program> pro;

	auto onInitialize = [&](Window& window){
		//ui.update_resolution(resolution);
		
		window.update_resolution(resolution);
		window.update_title("Voxel Engine");

		/*window.on_key_event(std::bind(&UI::forward_key_event, &ui, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		window.on_mouse_move_event(std::bind(&UI::forward_mouse_move_event, &ui, std::placeholders::_1, std::placeholders::_2));
		window.on_mouse_button_event(std::bind(&UI::forward_mouse_button_event, &ui, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		window.on_mouse_wheel_event(std::bind(&UI::forward_mouse_wheel_event, &ui, std::placeholders::_1, std::placeholders::_2));*/

		
		JSValue::Object api;
		api["player"] = player.create_ui_api();
		api["input"] = window.input().create_ui_api();
		api["editor"] = editor.create_ui_api();
		//ui.register_api(api);

		//window.input().on(Input::MoveableCombo(Input::MoveableTerminal::MOUSE), [&](Input& in){
		//	const auto delta = in.mouse().get_position_delta();
		//	player.rotate_yaw(delta.x * 0.15f);
		//	player.rotate_pitch(delta.y * 0.2f);
		//});

		//window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_2, Input::PressableEvent::RELEASED), [&](Input& in){
		//	Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
		//	if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
		//		chunkVault.delete_voxel(intersection->get_object_of_interest());
		//	}
		//});

		//window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [&](Input& in){
		//	Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
		//	if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
		//		chunkVault.add_adjacent_voxel(intersection->get_object_of_interest(), Color(0x00, 0x85, 0xAD, 255));
		//	}
		//});

		//window.input().mouse().lock_movement();

		std::string vertexPath = "C:/Users/sxenog/Documents/Projects/voxel_engine/src/shaders/test/test.vert";
		std::string fragmentPath = "C:/Users/sxenog/Documents/Projects/voxel_engine/src/shaders/test/test.frag";
		std::vector<glm::vec3> verts = {
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		};
		vao = std::make_shared<VertexArrayObject>();
		vao->addVertexAttribute<glm::vec3>();
		buffer = std::make_shared<VertexBufferObject>(Buffer::Usage::STATIC_DRAW, verts, vao);
		pro = std::make_shared<Program>(vertexPath, fragmentPath);
	};
	
	auto onIterate = [&](Window& window, float timeStepInSeconds){
		//ui.tick();
		/*player.tick(timeStepInSeconds);

		Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
		Renderer::clear_screen();
		if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
			const auto modelMatrix = chunkVault.get_voxel_model_matrix(intersection->get_object_of_interest());
			const auto mesh = chunkVault.get_mesh_of_voxel(intersection->get_object_of_interest());
			Renderer::render_wireframe(camera, modelMatrix, Color(0xFF, 0xFF, 0xFF, 255), mesh);
		}
		chunkVault.render(camera);*/
		//ui.render();
		/*Binder<Program> programBinder(*pro);
		Binder<VertexArrayObject> vaoBinder(*vao);
		Binder<Buffer> bufferBinder(*buffer);*/

		pro->bind();
		buffer->bind();

		Renderer::clear_screen();
		glDrawArrays(GL_TRIANGLES, 0, 3); gl_error_check();
	};

	auto onClosing = [&](Window& window) {
		pro.reset();
		vao.reset();
		buffer.reset();
		
	};

	

	MainLoop loop(onInitialize, onIterate, onClosing, 1.0f/200.0f);
    return 0;
}
