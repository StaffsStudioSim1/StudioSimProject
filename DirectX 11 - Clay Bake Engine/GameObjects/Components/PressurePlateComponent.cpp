#include "PressurePlateComponent.h"
#include "../PlayerController.h"
//include PlayerMagnetism.h



#include "../ObjectHandler.h"

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
	m_Position.y = _gameObject->GetTransform()->GetPosition().y + 0.5*m_Boxsize.y;
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

void PressurePlateComponent::Update()
{
	
	std::vector<GameObject*> areaCheck = ObjectHandler::GetInstance().GetObjectsInArea(m_Position, m_Boxsize);

	bool ObjectFound = false;

	for (GameObject* object : areaCheck)
	{
		//if (object->GetComponent<MagnetismObject>() != nullptr or object->GetComponent<PlayerController>())
		//{
		//	WeighedDown();
		//	ObjectFound = true;
		//}
		if (object->GetComponent<PlayerController>())
		{
			WeighedDown();
			ObjectFound = true;
		}

	}

	if (ObjectFound != true)
	{
		WeightReleased();
	}


	//check hitbox state
	//physics trigger volume
	// if volume occupied call weighed down, call weight released on volume not being occupied
	
}

