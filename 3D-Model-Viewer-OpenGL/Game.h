#pragma once
#include <SDL3/SDL.h>
#include <vector>

#include "Shader.h"
#include "Model.h"
#include "Renderer.h"

// TODO
// camera, model
// user interface

class Game
{
private:
	Game();

public:
	static Game* Instance();

	bool Initialize();
	void Runloop();
	void Shutdown();

	void AddActor(class Actor*);
	void RemoveActor(class Actor*);

	Renderer& GetRenderer() { return mRenderer; }
private:
	static Game* mInstance;
	void HandleInput();
	void Update();
	void Render();

	void LoadData();
	void UnloadData();

	SDL_Window* mWindow;
	SDL_GLContext mContext;

	bool mIsRunning;
	uint64_t mTicksCount;

	bool mUpdatingActors;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	Renderer mRenderer;

	// game
	class Camera* cam;
};

