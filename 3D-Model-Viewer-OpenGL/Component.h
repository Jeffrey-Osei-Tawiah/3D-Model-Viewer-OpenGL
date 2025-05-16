#pragma once
class Component
{
public:
	Component(class Actor* actor, int updateOrder);
	~Component();

	virtual void Update(float deltaTime){}

	int GetUpdateOrder()const { return mUpdateOrder; }
private:
	class Actor* mActor;
	int mUpdateOrder;
};

