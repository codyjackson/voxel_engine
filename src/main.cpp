#include "main_loop.h"

#include "rendering/camera.h"
#include "rendering/chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <GLFW/glfw3.h>

int main()
{
	auto onInitialize = [&](Window& window){
		window.update_width(1024);
		window.update_height(768);
		window.update_title("Voxel Engine");

		window.input().on(Input::PressableTerminal(Input::Pressable::ESCAPE, Input::PressableEvent::RELEASED), [&window](Input& in){
			window.close();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::PRESSED), [](Input& in){
			in.mouse().lock_movement();
		});

		window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [](Input& in){
			in.mouse().unlock_movement();
		});

		window.input().on(Input::MoveableCombo(Input::Pressable::MOUSE_BUTTON_1, Input::MoveableTerminal::MOUSE), [](Input& in){
			const auto delta = in.mouse().get_position_delta();
			std::cout << delta.x << " " << delta.y << std::endl;
		});
	};

	Orientation orientation;
	//orientation.rotate(Constants::Vec3::right, 0.0f);
	Camera camera(Constants::Vec3::up, orientation);

	Chunk<16> chunk(glm::vec3(0.0f, 0.0f, -5.0f));
	Chunk<16> chunk2(glm::vec3(15.0f, 0.0f, -5.0f));
	

	auto onIterate = [&](Window& window){
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glMultMatrixf(glm::value_ptr(camera.get_projection_matrix()));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(camera.get_view_matrix()));

		chunk.draw(glm::mat4());
		chunk2.draw(glm::mat4());

		window.draw();
	};
	MainLoop loop(onInitialize, onIterate);

    return 0;
}
