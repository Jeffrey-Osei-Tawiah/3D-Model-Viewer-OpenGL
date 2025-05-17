#include "Renderer.h"
#include <algorithm>
#include "ModelComponent.h"

void Renderer::AddModelComponent(ModelComponent* model)
{
	// TODO: load model's, textures and shaders using renderer
	// renderer stores array of models and textures and shaders
	mModels.push_back(model);
}

void Renderer::RemoveModelComponent(ModelComponent* model)
{
	auto iter = std::find(mModels.begin(), mModels.end(), model);

	if (iter != mModels.end())
	{
		std::iter_swap(iter, mModels.end() - 1);
		mModels.pop_back();
	}
}

void Renderer::Draw()
{
	for (ModelComponent* model : mModels)
	{
		model->Draw();
	}
}
