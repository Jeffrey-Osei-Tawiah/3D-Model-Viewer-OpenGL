#include "Game.h"

#define MyGame Game::Instance()
int main(int, char**)
{
	if (MyGame->Initialize())
	{
		MyGame->Runloop();
	}
	MyGame->Shutdown();
	return 0;
}