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
	m_Boxsize.x = 1.0f;
	m_Boxsize.y = 1.0f;


	m_Position.x = _gameObject->GetTransform()->GetPosition().x;
	m_Position.y = _gameObject->GetTransform()->GetPosition().y + 0.5 * m_Boxsize.y;
	m_WeighedDown = false;
}

void PressurePlateComponent::WeighedDown()
{
	SendSignal();
	m_WeighedDown = true;
}

void PressurePlateComponent::WeightReleased()
{
	SendSignal();
	m_WeighedDown = false;
}

json PressurePlateComponent::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "PressurePlateComponent";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(interactableLink);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_linkedObjectName);
	return me;
}

void PressurePlateComponent::Update(float deltaTime)
{

	std::vector<GameObject*> areaCheck = ObjectHandler::GetInstance().GetObjectsInArea(m_Position, m_Boxsize);

	bool ObjectFound = false;

	//check hitbox state
	for (GameObject* object : areaCheck)
	{
		if (object->GetComponent<MagnetismObject>() != nullptr || object->GetComponent<PlayerController>() != nullptr)
		{
			if (!m_WeighedDown)
				WeighedDown();
			ObjectFound = true;
		}
	}

	if (!ObjectFound && m_WeighedDown)
	{
		// call weight released on volume not being occupied
		WeightReleased();
	}
}

