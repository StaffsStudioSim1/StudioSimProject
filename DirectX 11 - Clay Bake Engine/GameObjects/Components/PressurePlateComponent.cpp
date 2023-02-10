#include "PressurePlateComponent.h"

PressurePlateComponent::PressurePlateComponent(InteractableLink switchType, std::string linkedObjectName) : Interactable(switchType, linkedObjectName)
{
	interactableLink = switchType;

	if (linkedObjectName != "")
	{
		linkedObject = ObjectHandler::GetInstance().FindGameObject(linkedObjectName);
	}
}

void PressurePlateComponent::Start()
{
	Interactable::Start();
	boxSize.x = 1.0f;
	boxSize.y = 1.0f;


	position.x = _gameObject->GetTransform()->GetPosition().x;
	position.y = _gameObject->GetTransform()->GetPosition().y + 0.5 * boxSize.y;
	weighedDown = false;
}

void PressurePlateComponent::WeighedDown()
{
	SendSignal();
	weighedDown = true;
}

void PressurePlateComponent::WeightReleased()
{
	SendSignal();
	weighedDown = false;
}

json PressurePlateComponent::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "PressurePlateComponent";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(interactableLink);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(linkedObjectName);
	return me;
}

void PressurePlateComponent::Update(float deltaTime)
{

	std::vector<GameObject*> areaCheck = ObjectHandler::GetInstance().GetObjectsInArea(position, boxSize);

	bool ObjectFound = false;

	//check hitbox state
	for (GameObject* object : areaCheck)
	{
		if (object->GetComponent<MagnetismObject>() != nullptr || object->GetComponent<PlayerController>() != nullptr)
		{
			if (!weighedDown)
				WeighedDown();
			ObjectFound = true;
		}
	}

	if (!ObjectFound && weighedDown)
	{
		// call weight released on volume not being occupied
		WeightReleased();
	}
}

