#include "Actor.h"
#include "Game.h"

Actor::Actor()
	:mPosition(glm::vec3(0.f)), mScale(glm::vec3(0.f)), mRotation(glm::quat(1.0, 0, 0, 0)), 
	mRecomputeModelMatrix(false), mState(EActive)
{
	Game::Instance()->AddActor(this);
}

Actor::~Actor()
{
	// delete components

	Game::Instance()->RemoveActor(this);
}

void Actor::AddComponent(Component* component)
{
}

void Actor::RemoveComponent(Component* component)
{
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{

}
