#include "window.h"

#include "chunk.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

int main(void)
{
	Window win(1024, 720);
	win.open();
	Chunk<16> chunk(glm::vec3(0,0,0));
	Chunk<16> chunk2(glm::vec3(15,0,0));
    /* Loop until the user closes the window */
	while (win.is_open())
    {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-0.5f, -0.5f, -0.5f);
		glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);

		chunk.draw(glm::mat4());
		chunk2.draw(glm::mat4());
        /* Poll for and process events */
		win.draw();
        glfwPollEvents(); 
    }

    glfwTerminate();
    return 0;
}
