#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
	:Actor()
{
	mView = glm::mat4(1.0f);
	mCamDir = glm::vec3(0, 0, -1);
	mUp = glm::vec3(0, 1, 0);
	mMouseDelta = glm::vec2();
	angleX = 0;
	angleY = 0;
	mMoveAxis = glm::vec2(0.0f);
}

void Camera::OnMouseMotion(const glm::vec2& motion)
{
	float sensitivity = 20;
	glm::vec2 delta = sensitivity * motion;

	mMouseDelta = delta;
}

void Camera::ActorInput(const glm::vec2& movementAxis)
{
	mMoveAxis = movementAxis;
}

void Camera::UpdateActor(float deltaTime)
{
	angleX += mMouseDelta.x * deltaTime;
	angleY += mMouseDelta.y * deltaTime;
	if (glm::abs(angleY) > 60)
		angleY = 60 * glm::sign(angleY);

	SetRotation(glm::angleAxis(glm::radians(angleX), glm::vec3(0, 1, 0)));
	SetRotation(GetRotation() * glm::angleAxis(glm::radians(angleY), glm::vec3(1, 0, 0)));


	glm::vec3 target = GetRotation() * mCamDir;

	mView = glm::lookAt(GetPosition(), GetPosition() + target, mUp);
	mMouseDelta = glm::vec2(0);

	float speed = 30 * deltaTime;
	glm::vec3 moveDelta = speed * mMoveAxis.y * glm::vec3(target.x, 0, target.z);
	moveDelta += speed * mMoveAxis.x * glm::cross(target, mUp);
	SetPosition(GetPosition() + moveDelta);
}
