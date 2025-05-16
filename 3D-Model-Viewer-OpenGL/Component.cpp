#include "Component.h"
#include "Actor.h"

Component::Component(Actor* actor, int updateOrder)
	:mActor(actor), mUpdateOrder(updateOrder)
{
	mActor->AddComponent(this);
}

Component::~Component()
{
	mActor->RemoveComponent(this);
}
