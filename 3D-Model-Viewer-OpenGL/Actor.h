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
	virtual ~Actor();

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);

	virtual void UpdateActor(float deltaTime){}

	// Getters/Setters
	State GetState()const { return mState; }
	void SetState(State state) { mState = state; }

	void SetPosition(float x = 0, float y = 0, float z = 0) { mPosition = glm::vec3(x, y, z); mRecomputeModelMatrix = true; }
	void SetPosition(const glm::vec3& pos) { mPosition = pos; mRecomputeModelMatrix = true; }
	const glm::vec3& GetPosition()const { return mPosition; }

	void SetScale(float x = 1, float y = 1, float z = 1) { mScale = glm::vec3(x, y, z); mRecomputeModelMatrix = true; }
	void SetScale(const glm::vec3& scale) { mScale = scale; mRecomputeModelMatrix = true; }
	const glm::vec3& GetScale()const { return mScale; }

	void SetRotation(float w = 1, float x = 0, float y = 0, float z = 0) { mRotation = glm::quat(w, x, y, z); mRecomputeModelMatrix = true; }
	void SetRotation(const glm::quat& q) { mRotation = q; mRecomputeModelMatrix = true; }
	const glm::quat& GetRotation()const { return mRotation; }

	const glm::mat4& GetModelMatrix()const { return mModelMatrix; }

	// Probably Temp
	// On event functions
	virtual void OnMouseMotion(const glm::vec2& motion){}
	virtual void ActorInput(const glm::vec2& movementAxis){}

private:
	State mState;
	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::quat mRotation;

	bool mRecomputeModelMatrix;
	glm::mat4 mModelMatrix;
	std::vector<class Component*> mComponents;
};

