#include "main_loop.h"

#include "chunk.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

int main(void)
{
	auto onInitialize = [&](Window& window){
		window.update_width(1024);
		window.update_height(768);
		window.update_title("Voxel Engine");
	};

	Chunk<16> chunk(glm::vec3(0.0f, 0.0f, 0.0f));
	Chunk<16> chunk2(glm::vec3(15.0f, 0.0f, 0.0f));

	auto onIterate = [&](Window& window){
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-0.5f, -0.5f, -0.5f);
		glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);

		chunk.draw(glm::mat4());
		chunk2.draw(glm::mat4());

		window.draw();
	};
	MainLoop loop(onInitialize, onIterate);

    return 0;
}
