#include "Game.h"
#include <GL/glew.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Actor.h"
#include "ModelComponent.h"


#define FPS 60

Game* Game::mInstance = NULL;

Game::Game()
	:mWindow(NULL), mContext(NULL), mIsRunning(false), mTicksCount(0), mUpdatingActors(false)
{

}

Game* Game::Instance()
{
	if (!mInstance)
	{
		mInstance = new Game();
	}

	return mInstance;
}

bool Game::Initialize()
{
	// Initialize SDL
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to Initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// Set up window openGL configurations
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);

	// create window
	if (!(mWindow = SDL_CreateWindow("3D Model Loader", 800, 600, SDL_WINDOW_OPENGL)))
	{
		SDL_Log("Failed to Create Window: %s\n", SDL_GetError());
		return false;
	}

	// Set openGL Context
	mContext = SDL_GL_CreateContext(mWindow);

	if (!mContext)
	{
		SDL_Log("Failed to create openGL context: %s\n", SDL_GetError());
		return false;
	}

	// initialize glew
	GLenum result = glewInit();

	if (result != GLEW_OK)
	{
		SDL_Log("Failed to initialize glew: %s\n", glewGetErrorString(result));
		return false;
	}

	SDL_Log("Using GLEW %s\n", glewGetString(GLEW_VERSION));

	mIsRunning = true;
	mTicksCount = SDL_GetTicks();
	LoadData();
	return true;
}

void Game::Runloop()
{
	while (mIsRunning)
	{
		HandleInput();
		Update();
		Render();
	}
}

void Game::Shutdown()
{
	UnloadData();
	SDL_GL_DestroyContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	delete mInstance;
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.push_back(actor);
	}
	else
	{
		mActors.push_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);

	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
		return;
	}

	iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
}

void Game::HandleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			mIsRunning = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.key == SDLK_ESCAPE)
				mIsRunning = false;
			break;
		}
	}
}

void Game::Update()
{
	// implement fixed framerate
	while ((SDL_GetTicks() - mTicksCount) < (1.0f / FPS) * 1000)
	{
		SDL_Delay(((1.0f / FPS) * 1000) - (SDL_GetTicks() - mTicksCount));
	}

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	mUpdatingActors = true;
	for (Actor* actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for (Actor* actor : mPendingActors)
	{
		mActors.push_back(actor);
	}
	mPendingActors.clear();

	for (auto iter = mActors.begin(); iter != mActors.end(); iter++)
	{
		if ((*iter)->GetState() == Actor::EDead)
		{
			delete (*iter);
			iter--;
		}
	}
}

void Game::Render()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mRenderer.Draw();

	SDL_GL_SwapWindow(mWindow);
}

void Game::LoadData()
{
	glEnable(GL_DEPTH_TEST);

	// test
	Actor* testActor = new Actor();
	ModelComponent* backpack = new ModelComponent(testActor, "backpack/backpack.obj", "shader.vert", "shader.frag");

	testActor->SetPosition(0, 0, -10.0f);
}

void Game::UnloadData()
{
	for (Actor* actor : mActors)
	{
		delete actor;
	}
}


