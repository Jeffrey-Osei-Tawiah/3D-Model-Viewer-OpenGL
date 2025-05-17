#include "ModelComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

ModelComponent::ModelComponent(Actor* actor, const std::string& modelFileName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	:Component(actor, 100)
{
	mModel = new Model(modelFileName);
	mShader = Shader(vertexShaderPath, fragmentShaderPath);

	Game::Instance()->GetRenderer().AddModelComponent(this);
}

ModelComponent::~ModelComponent()
{
	Game::Instance()->GetRenderer().RemoveModelComponent(this);
	delete mModel;
}

void ModelComponent::Draw()
{
	glm::mat4 model = mActor->GetModelMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);


	mShader.Use();
	mShader.SetMat4("model", model);
	mShader.SetMat4("view", glm::mat4(1.0f));
	mShader.SetMat4("projection", proj);

	mModel->Draw(mShader);


}
