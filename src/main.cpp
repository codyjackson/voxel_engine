#include <Windows.h>
#include "main_loop.h"

#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "spatial/chunk_vault.h"
#include "rendering/ui.h"
#include "units.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <memory>

int main(int argc, char* argv[])
{
	Orientation orientation;
	Camera camera(glm::vec3(), orientation);

	float metersPerSecondForward = 0.0f;
	float metersPerSecondRight = 0.0f;

	ChunkVault chunkVault(meters(2.0f / 13.0f), glm::vec3(0, 0, 0));

	std::shared_ptr<UI> ui;
	auto onInitialize = [&](Window& window){
		window.update_width(1024);
		window.update_height(768);
		window.update_title("Voxel Engine");

		window.input().mouse().hide_cursor();
		window.input().on(Input::PressableTerminal(Input::Pressable::ESCAPE, Input::PressableEvent::RELEASED), [&window](Input& in){
			window.close();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::W, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondForward = meters(3.0f);
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::W, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondForward = 0.0f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::S, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondForward = -meters(3.0f);
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::S, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondForward = 0.0f;
		});



		window.input().on(Input::PressableTerminal(Input::Pressable::D, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondRight = meters(3.0f);
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::D, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondRight = 0.0f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::A, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondRight = -meters(3.0f);
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::A, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondRight = 0.0f;
		});

		window.input().on(Input::MoveableCombo(Input::MoveableTerminal::MOUSE), [&](Input& in){
			const auto delta = in.mouse().get_position_delta();
			camera.orientation.rotate(Constants::Vec3::up, delta.x*0.15f);
			camera.orientation.rotate(camera.orientation.right(), delta.y*0.2f);
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_2, Input::PressableEvent::RELEASED), [&](Input& in){
			Ray r(camera.position, camera.orientation.forward());
			if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
				chunkVault.delete_voxel(intersection->get_object_of_interest());
			}
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [&](Input& in){
			Ray r(camera.position, camera.orientation.forward());
			if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
				chunkVault.add_adjacent_voxel(intersection->get_object_of_interest(), Color(0x00, 0x85, 0xAD, 255));
			}
		});

		window.input().mouse().lock_movement();
		ui = std::make_shared<UI>(1000, 1000);
	};
	
	auto onIterate = [&](Window& window, float timeStepInSeconds){
		camera.position += timeStepInSeconds * metersPerSecondForward * camera.orientation.forward();
		camera.position += timeStepInSeconds * metersPerSecondRight * camera.orientation.right();

		Ray r(camera.position, camera.orientation.forward());
		Renderer::clear_screen();
		if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
			const auto modelMatrix = chunkVault.get_voxel_model_matrix(intersection->get_object_of_interest());
			const auto mesh = chunkVault.get_mesh_of_voxel(intersection->get_object_of_interest());
			Renderer::render_wireframe(camera, modelMatrix, Color(0xFF, 0xFF, 0xFF, 255), mesh);
		}
		chunkVault.render(camera);
		ui->tick();
	};


	MainLoop loop(onInitialize, onIterate, 1.0f/40.0f);
    return 0;
}
