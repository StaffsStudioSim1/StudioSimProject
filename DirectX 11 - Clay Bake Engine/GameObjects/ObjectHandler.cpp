#include "ObjectHandler.h"

ObjectHandler::ObjectHandler()
{

}

void ObjectHandler::CreateGameObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 scale, float rotation, bool hasPhysics, std::string textureName, DirectX::XMFLOAT4 texCoords)
{
	GameObject* tempObject = new GameObject(0);
	tempObject->AddAppearance(new Appearance);
	tempObject->AddTransform(new Transform);

	// Set transformation values
	tempObject->GetTransform()->SetPosition(position);
	tempObject->GetTransform()->SetScale(scale);
	tempObject->GetTransform()->SetRotation(rotation);

	// Set appearance values
	tempObject->GetAppearance()->SetTexture(GetLoadedTexture(textureName));
	tempObject->GetAppearance()->SetTexCoords(texCoords);
	tempObject->GetAppearance()->SetGeometryData(GetSquareGeometry());

	// Add physics if needed
	if (hasPhysics)
	{
		tempObject->AddPhysics(new Physics(tempObject->GetTransform()));
	}

	// Add to map
	AddGameObjectToMap(name, tempObject);
}

void ObjectHandler::RemoveGameObject(std::string name)
{
	GameObject* objectToDelete = _gameObjects[name];
	_gameObjects.erase(name);
	delete objectToDelete;
	objectToDelete = nullptr;
}

void ObjectHandler::ClearGameObjects()
{
	std::unordered_map<std::string, GameObject*> objectsToDelete = _gameObjects;
	_gameObjects.clear();
	for (std::pair<std::string, GameObject*> object : objectsToDelete)
	{
		delete object.second;
		object.second = nullptr;
	}
}
