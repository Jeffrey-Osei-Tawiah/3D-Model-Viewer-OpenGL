#pragma once
#include <string>

#include "Component.h"
#include "Shader.h"
#include "Model.h"
class ModelComponent : protected Component
{
public:
	ModelComponent(class Actor* actor,
		const std::string& modelFileName,
		const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath);

	~ModelComponent();

	// TODO: add camera class and use as parameter for Draw()
	void Draw(class Camera* cam);
private:
	Shader mShader;
	Model* mModel;
};

