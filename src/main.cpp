#include "game.h"
#include "main_loop.h"




int main(int argc, char* argv[])
try
{
	MainLoop<Game> mainLoop(1.0f/200.0f);
    return 0;
}
catch (std::exception ex)
{
	std::cout << ex.what() << std::endl;
	throw;
}