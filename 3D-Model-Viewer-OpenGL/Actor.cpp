#include "Actor.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Actor::Actor()
	:mRecomputeModelMatrix(false),
	mPosition(glm::vec3(0.0f)),
	mScale(glm::vec3(1.0f)),
	mRotation(glm::quat(1.0f, 0, 0, 0)),
	mModelMatrix(glm::mat4(1.0f)),
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
		if (mRecomputeModelMatrix)
		{
			mModelMatrix = glm::scale(glm::mat4(1.0f), mScale);
			glm::mat4 rot = glm::mat4_cast(mRotation);
			mModelMatrix = rot * mModelMatrix;
			mModelMatrix = glm::translate(mModelMatrix, mPosition);

			mRecomputeModelMatrix = false;
		}

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		if (mRecomputeModelMatrix)
		{
			mModelMatrix = glm::scale(glm::mat4(1.0f), mScale);
			glm::mat4 rot = glm::mat4_cast(mRotation);
			mModelMatrix = rot * mModelMatrix;
			mModelMatrix = glm::translate(mModelMatrix, mPosition);

			mRecomputeModelMatrix = false;
		}
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (Component* comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}
