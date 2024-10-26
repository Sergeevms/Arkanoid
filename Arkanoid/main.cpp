#include "MainLoop.h"


int main()
{
	const unsigned seed = static_cast<unsigned int>(time(nullptr));
	srand(seed);
	SnakeGame::MainLoop mainLoop;
	mainLoop.Run();
	return 0;
}
