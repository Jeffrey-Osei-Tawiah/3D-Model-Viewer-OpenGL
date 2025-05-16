#include "Actor.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>

Actor::Actor()
	:mRecomputeModelMatrix(false),
	Position(glm::vec3(0.0f), this),
	Scale(glm::vec3(1.0f), this),
	Rotation(glm::quat(1.0f, 0, 0, 0), this),
	mState(EActive)
{
	Game::Instance()->AddActor(this);
}
Actor::~Actor()
{
	for (Component* comp : mComponents)
	{
		delete comp;
	}
	mComponents.clear();

	Game::Instance()->RemoveActor(this);
}

void Actor::AddComponent(Component* component)
{
	int updateOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if ((*iter)->GetUpdateOrder() > updateOrder)
			break;
	}

	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	// could probably be optimized because mComponents is sorted
	// TODO: Optimize this function
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);

	if (iter != mComponents.end())
	{
		std::iter_swap(iter, mComponents.end() - 1);
		mComponents.pop_back();
	}
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
	for (Component* comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}
