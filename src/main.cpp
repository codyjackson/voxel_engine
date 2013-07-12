#include "window.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

int main(void)
{
	Window win(1024, 720);
	Bitmap scene(1024, 720, Color(0, 255,0));
	win.open();
    /* Loop until the user closes the window */
	while (win.is_open())
    {
        /* Poll for and process events */
		win.draw(scene);
        glfwPollEvents(); 
    }

    glfwTerminate();
    return 0;
}
