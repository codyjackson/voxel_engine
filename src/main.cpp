#include "window.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

int main(void)
{
	Window win(1024, 720);
	Window win1("testing1");
	win.open();
	win1.open();
    /* Loop until the user closes the window */
	while (win.is_open())
    {
        /* Poll for and process events */
        glfwPollEvents(); 
    }
	win.close();
	win1.hide();
	Window win2("testing again");
	win2.open();
	while(win2.is_open())
	{
		glfwPollEvents();
	}
	win1.open();
	while(win1.is_open())
	{
		glfwPollEvents();
	}
    glfwTerminate();
    return 0;
}
