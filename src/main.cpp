#include <Windows.h>
#include "main_loop.h"

#include "player.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "spatial/chunk_vault.h"
#include "ui.h"
#include "units.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <memory>

int main(int argc, char* argv[])
{
	Player player;
	Camera camera(Transform::make_transform(glm::vec3(), Orientation(), player.get_transform()));

	ChunkVault chunkVault(meters(2.0f / 13.0f), glm::vec3(0, 0, 0));

	auto onInitialize = [&](Window& window){
		window.update_resolution(RectSize(1024, 768));
		window.update_title("Voxel Engine");

		window.input().mouse().hide_cursor();
		window.input().on(Input::PressableTerminal(Input::Pressable::ESCAPE, Input::PressableEvent::RELEASED), [&window](Input& in){
			window.close();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::W, Input::PressableEvent::PRESSED), [&](Input& in){
			player.start_moving_forward();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::W, Input::PressableEvent::RELEASED), [&](Input& in){
			player.stop_moving_forward_or_backward();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::S, Input::PressableEvent::PRESSED), [&](Input& in){
			player.start_moving_backward();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::S, Input::PressableEvent::RELEASED), [&](Input& in){
			player.stop_moving_forward_or_backward();
		});



		window.input().on(Input::PressableTerminal(Input::Pressable::D, Input::PressableEvent::PRESSED), [&](Input& in){
			player.start_moving_right();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::D, Input::PressableEvent::RELEASED), [&](Input& in){
			player.stop_moving_left_or_right();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::A, Input::PressableEvent::PRESSED), [&](Input& in){
			player.start_moving_left();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::A, Input::PressableEvent::RELEASED), [&](Input& in){
			player.stop_moving_left_or_right();
		});

		window.input().on(Input::MoveableCombo(Input::MoveableTerminal::MOUSE), [&](Input& in){
			const auto delta = in.mouse().get_position_delta();
			player.rotate_yaw(delta.x * 0.15f);
			player.rotate_pitch(delta.y * 0.2f);
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_2, Input::PressableEvent::RELEASED), [&](Input& in){
			Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
			if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
				chunkVault.delete_voxel(intersection->get_object_of_interest());
			}
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [&](Input& in){
			Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
			if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
				chunkVault.add_adjacent_voxel(intersection->get_object_of_interest(), Color(0x00, 0x85, 0xAD, 255));
			}
		});

		window.input().mouse().lock_movement();
	};
	
	auto onIterate = [&](Window& window, float timeStepInSeconds){
		player.tick(timeStepInSeconds);

		Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
		Renderer::clear_screen();
		if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
			const auto modelMatrix = chunkVault.get_voxel_model_matrix(intersection->get_object_of_interest());
			const auto mesh = chunkVault.get_mesh_of_voxel(intersection->get_object_of_interest());
			Renderer::render_wireframe(camera, modelMatrix, Color(0xFF, 0xFF, 0xFF, 255), mesh);
		}
		chunkVault.render(camera);
	};


	MainLoop loop(onInitialize, onIterate, 1.0f/40.0f);
    return 0;
}
