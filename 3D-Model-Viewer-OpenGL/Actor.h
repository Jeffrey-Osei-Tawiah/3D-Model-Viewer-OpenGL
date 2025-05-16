#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/quaternion.hpp>

class Actor
{
public:
	enum State
	{
		EActive,
		EPause,
		EDead
	};
	Actor();
	~Actor();

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);

	virtual void UpdateActor(float deltaTime){}

	// Getters/Setters
	State GetState()const { return mState; }
private:
	State mState;

	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::quat mRotation;

	bool mRecomputeModelMatrix;
	std::vector<class Component*> mComponents;
};

