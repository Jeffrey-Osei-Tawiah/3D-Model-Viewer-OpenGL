#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/quaternion.hpp>

class Actor
{
private:
	// Getters and Setters similar to C#
	// no need to make pos,scal and rotation
	// private
	template<typename T>
	class Property
	{
	public:
		Property() = default;
		Property(const T& val, Actor* actor) : mValue(val), mActor(actor) {}

		T& operator=(const T& val)
		{
			mValue = val;
			mActor->mRecomputeModelMatrix = true;
		}

		operator T() const {
			return mValue;
		}
	private:
		T mValue;
		Actor* mActor;
	};

public:
	Property<glm::vec3> Position;
	Property<glm::vec3> Scale;
	Property<glm::quat> Rotation;

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

	virtual void UpdateActor(float deltaTime) {}

	// Getters/Setters
	State GetState()const { return mState; }
	void SetState(State state) { mState = state; }
private:
	State mState;

	bool mRecomputeModelMatrix;
	std::vector<class Component*> mComponents;
};

