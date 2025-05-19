#pragma once
#include "Actor.h"


class Camera :	public Actor
{
public:
	Camera();

	void OnMouseMotion(const glm::vec2& motion)override;
	void ActorInput(const glm::vec2& movementAxis)override;
	void UpdateActor(float deltaTime)override;
	const glm::mat4& GetView()const { return mView; }
private:
	glm::mat4 mView;

	glm::vec3 mCamDir;
	glm::vec3 mUp;
	glm::vec2 mMouseDelta;
	float angleX;
	float angleY;
	glm::vec2 mMoveAxis;
};

