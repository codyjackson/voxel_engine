#include "main_loop.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

int main(void)
{
	Bitmap scene(1024, 768, Color(0, 255,0));
	auto onInitialize = [&](Window& window){
		window.update_width(1024);
		window.update_height(768);
		window.update_title("Voxel Engine");
	};
	auto onIterate = [&](Window& window){
		window.draw(scene);
	};
	MainLoop loop(onInitialize, onIterate);

    return 0;
}
