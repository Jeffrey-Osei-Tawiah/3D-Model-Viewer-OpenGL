#include "Game.h"
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define FPS 60

Game* Game::mInstance = NULL;

Game::Game()
	:mWindow(NULL), mContext(NULL), mIsRunning(false), mTicksCount(0)
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
	SDL_GL_DestroyContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor*)
{
}

void Game::RemoveActor(Actor*)
{
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


	// update actors
	//delete actors

	angle += 30 * deltaTime;
}

void Game::Render()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10));
	m = glm::rotate(m, glm::radians(angle), glm::vec3(0, 1, 0));
	m = glm::scale(m, glm::vec3(1, 1, 1));

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);

	sh.Use();
	sh.SetMat4("model", m);
	sh.SetMat4("view", view);
	sh.SetMat4("projection", proj);

	model->Draw(sh);

	glUseProgram(0);

	SDL_GL_SwapWindow(mWindow);
}

void Game::LoadData()
{
	sh = Shader("shader.vert", "shader.frag");
	model = new Model("backpack/backpack.obj");

	glEnable(GL_DEPTH_TEST);
}

void Game::UnloadData()
{
}


