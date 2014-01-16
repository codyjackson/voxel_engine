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

		const auto mouseMove = Input::MoveableCombo(Input::Moveable::MOUSE);
		window.input().on(mouseMove, [](Input& in){
			auto m = in.mouse().get_position();
			std::cout << m.x << " " << m.y << std::endl; 
		});

		const auto scrollMove = Input::MoveableCombo(Input::Moveable::MOUSE_WHEEL);
		window.input().on(scrollMove, [](Input& in){
			std::cout << in.mouse().get_wheel_delta() << std::endl;
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
