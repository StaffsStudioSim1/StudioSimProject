#include "Component.h"
#include "GameObject.h"

void Component::SetObject(GameObject* gameObject)
{
	_gameObject = gameObject;
}

void Component::Start()
{
}

void Component::Update(float deltaTime)
{
}

void Component::FixedUpdate(float timeStep)
{
}

void Component::Stop()
{
}
