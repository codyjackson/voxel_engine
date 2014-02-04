#include "main_loop.h"

#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "spatial/chunk_vault.h"
#include "units.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

int main()
{
	Orientation orientation;
	Camera camera(glm::vec3(), orientation);

	float metersPerSecondForward = 0.0f;
	float metersPerSecondRight = 0.0f;

	ChunkVault chunkVault(meters(2.0f / 16.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 4);

	auto onInitialize = [&](Window& window){
		window.update_width(1024);
		window.update_height(768);
		window.update_title("Voxel Engine");

		window.input().on(Input::PressableTerminal(Input::Pressable::ESCAPE, Input::PressableEvent::RELEASED), [&window](Input& in){
			window.close();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::W, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondForward = 0.01f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::W, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondForward = 0.0f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::S, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondForward = -0.01f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::S, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondForward = 0.0f;
		});



		window.input().on(Input::PressableTerminal(Input::Pressable::D, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondRight = 0.01f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::D, Input::PressableEvent::RELEASED), [&](Input& in){
			metersPerSecondRight = 0.0f;
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::A, Input::PressableEvent::PRESSED), [&](Input& in){
			metersPerSecondRight = -0.01f;
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
			if (const auto intersection = chunkVault.find_nearest_intersection(r))
				chunkVault.delete_voxel(intersection->get_object_of_interest());
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [&](Input& in){
			Ray r(camera.position, camera.orientation.forward());
			if (const auto intersection = chunkVault.find_nearest_intersection(r))
				chunkVault.add_adjacent_voxel(intersection->get_object_of_interest(), Color(0, 0, 0, 255));
		});

		window.input().mouse().lock_movement();
	};
	
	auto onIterate = [&](Window& window){
		camera.position += metersPerSecondForward * camera.orientation.forward();
		camera.position += metersPerSecondRight * camera.orientation.right();

		Ray r(camera.position, camera.orientation.forward());
		Renderer::clear_screen();

		const auto chunkRenderable = chunkVault.get_renderables();
		const auto render = [&camera](const Renderable& renderable){
			Renderer::render(camera, renderable);
			Renderer::render_wireframe(camera, Color(0x0C, 0x22, 0x33, 255), renderable);
		};
		std::for_each(std::begin(chunkRenderable), std::end(chunkRenderable), render);

		if (const auto intersection = chunkVault.find_nearest_intersection(r))
			Renderer::render_wireframe(camera, glm::mat4(), Color(0xF2, 0xF2, 0xF2, 255), chunkVault.get_mesh_of_voxel(intersection->get_object_of_interest()));
	};


	MainLoop loop(onInitialize, onIterate);
    return 0;
}
