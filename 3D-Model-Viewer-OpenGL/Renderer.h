#pragma once
#include <vector>
class Renderer
{
public:
	Renderer(){}
	//~Renderer();

	void AddModelComponent(class ModelComponent* model);
	void RemoveModelComponent(class ModelComponent* model);

	void Draw();
private:
	std::vector<class ModelComponent*> mModels;
};

