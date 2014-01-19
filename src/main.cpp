#include "main_loop.h"

#include "rendering/camera.h"
#include "rendering/chunk.h"
#include "units.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <GLFW/glfw3.h>

int main()
{
	Orientation orientation;
	Camera camera(glm::vec3(), orientation);

	float metersPerSecondForward = 0.0f;
	float metersPerSecondRight = 0.0f;

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



		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::PRESSED), [](Input& in){
			in.mouse().lock_movement();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [](Input& in){
			in.mouse().unlock_movement();
		});

		window.input().on(Input::MoveableCombo(Input::Pressable::MOUSE_BUTTON_1, Input::MoveableTerminal::MOUSE), [&](Input& in){
			const auto delta = in.mouse().get_position_delta();
			camera.orientation.rotate(Constants::Vec3::up, delta.x*0.15f);
			camera.orientation.rotate(camera.orientation.right(), delta.y*0.2f);
			std::cout << delta.x << " " << delta.y << std::endl;
		});
	};

	Chunk<16> chunk(glm::vec3(meters(4.0f), meters(-4.0f), meters(-6.0f)), meters(5.0f));
	
	auto onIterate = [&](Window& window){
		camera.position += metersPerSecondForward * camera.orientation.forward();
		camera.position += metersPerSecondRight * camera.orientation.right();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(camera.get_projection_matrix()));
		glMultMatrixf(glm::value_ptr(camera.get_view_matrix()));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glMultMatrixf(glm::value_ptr(camera.get_view_matrix()));

		chunk.draw(glm::mat4());

		window.draw();
	};
	MainLoop loop(onInitialize, onIterate);

    return 0;
}
