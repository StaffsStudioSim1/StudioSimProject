#include "Scene.h"
#include <fstream>
#include "ErrorLogger.h"
#include "nlohmann/json.hpp"
#include "GameObjects/ObjectHandler.h"
using json = nlohmann::json;

#if EDIT_MODE
#include "Input/InputManager.h"
#endif

Scene::Scene(std::string filePath)
{
	std::ifstream f(filePath);
	if (!f.good())
		ErrorLogger::Log("Unable to find scene file " + filePath);

	json data = json::parse(f);

	std::string imagePath = data[JSON_SCENE_BACKGROUND];
	_backgroundImage = new GameObject((std::string)JSON_SCENE_BACKGROUND);
	_backgroundImage->GetTransform()->SetDepthPos(1.0f);
	_backgroundImage->AddComponent(new Appearance(imagePath));

	for (json objectData : data[JSON_SCENE_GAMEOBJECTS])
		_children.push_back(new GameObject(objectData));

#if EDIT_MODE
	RECT rc;
	GetWindowRect(GetActiveWindow(), &rc);
	int width = (rc.right - rc.left) - 16;
	int height = (rc.bottom - rc.top) - 39;
	_mousePicking.Initialise(width, height);
	_geometry = ObjectHandler::GetInstance().GetSquareGeometry();
	_fileName = filePath;

	_prefabs.push_back(Prefab("Collision", "Resources/Sprites/StageCollision.dds", { 1.0f, 1.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"tag\" : \"StageCollision\", \"scale\" : [1.0, 1.0], \"components\" : [ { \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/StageCollision.dds\", 1.0, 1.0, 0.0, 0.0, 1.0] } ] }"));
	_prefabs.push_back(Prefab("Player Blue", "Resources/Sprites/Player1.dds", { 6.0f, 8.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Player1.dds\", 6.0, 8.0, 0.0, 0.0, 1.0] }, { \"class\" : \"PlayerController\", \"constructors\" : [1] }] }"));
	_prefabs.push_back(Prefab("Player Red", "Resources/Sprites/Player2.dds", { 6.0f, 8.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Player2.dds\", 6.0, 8.0, 0.0, 0.0, 1.0] }, { \"class\" : \"PlayerController\", \"constructors\" : [2] }] }"));
	_prefabs.push_back(Prefab("Box", "Resources/Sprites/Box.dds", { 1.0f, 1.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Box.dds\", 1.0, 1.0, 0.0, 0.0, 1.0] }] }"));
	_prefabs.push_back(Prefab("Lamp", "Resources/Sprites/Lamp.dds", { 2.0f, 1.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Lamp.dds\", 2.0, 1.0, 0.0, 0.0, 1.0, 0.0, 18.0] }] }"));

	_texture = ObjectHandler::GetInstance().LoadDDSTextureFile(_prefabs[_prefabNum].ghostImageFilepath);
#endif
}

Scene::~Scene()
{
	for (GameObject* obj : _children)
		delete obj;
	_children.clear();

	delete _backgroundImage;
}

void Scene::Save()
{
#if EDIT_MODE
	json scene;
	json gameObjects;

	//scene[JSON_SCENE_BACKGROUND] = "Resources/Textures/ZoeLevel.dds";
	scene[JSON_SCENE_BACKGROUND] = ObjectHandler::GetInstance().GetGameObject(0)->GetComponent<Appearance>()->GetTexture().filePath; // Presumes that the first object is the background

	for (GameObject* obj : _children)
	{
		if (obj->GetTag() == JSON_TAG_GAMEOBJECT)
			gameObjects.push_back(obj->Write());
		else if (obj->GetTag() == JSON_TAG_STAGECOLLISION)
		{

		}
	}

	scene["gameObjects"] = gameObjects;

	std::ofstream o(_fileName);
	o << std::setw(4) << scene << std::endl;
	o.close();
#endif
}

void Scene::Start()
{
	for (GameObject* obj : _children)
		obj->Start();
}

void Scene::Update(float deltaTime)
{
#if EDIT_MODE
	static GameObject* selectedObj = nullptr;
	static DirectX::XMINT2 startingPos = { 0, 0 };

	MouseClass* mouse = InputManager::GetInstance().GetMouse();
	DirectX::XMINT2 mousePos = { mouse->GetPosX(), mouse->GetPosY() };

	DirectX::XMINT2 relPos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
	relPos = _mousePicking.SnapCoordinatesToGrid(relPos.x, relPos.y);
	_ghost.x = relPos.x;
	_ghost.y = relPos.y;

	if (selectedObj)
	{
		// If an object is following the cursor
		DirectX::XMINT2 relativeMousePos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
		selectedObj->GetTransform()->SetPosition(relativeMousePos.x, relativeMousePos.y);
	}

	while (!mouse->EventBufferIsEmpty()) // Handles moving, creating and deleting objects with the mouse in edit mode
	{
		MouseEvent me = mouse->ReadEvent();
		switch (me.GetType())
		{
		case MouseEvent::EventType::LPress:
			if (!selectedObj)
			{
				// Pickup Tile
				selectedObj = _mousePicking.TestForObjectIntersection(mousePos.x, mousePos.y);
				if (selectedObj)
				{
					Vector2 pos = selectedObj->GetTransform()->GetPosition();
					startingPos = { (int)pos.x, (int)pos.y };
				}
			}
			break;
		case MouseEvent::EventType::LRelease:
			if (selectedObj)
			{
				// Drop tile at position
				Vector2 objectPos = selectedObj->GetTransform()->GetPosition();
				DirectX::XMINT2 snapPos = _mousePicking.SnapCoordinatesToGrid(objectPos.x, objectPos.y);

				for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
				{
					if (object != selectedObj && object->GetTransform()->GetPosition().x == snapPos.x && object->GetTransform()->GetPosition().y == snapPos.y)
					{
						selectedObj->GetTransform()->SetPosition(startingPos.x, startingPos.y);
						selectedObj = nullptr;
						return;
					}
				}

				selectedObj->GetTransform()->SetPosition(snapPos.x, snapPos.y);
				selectedObj = nullptr;
			}
			break;
		case MouseEvent::EventType::MPress:
			if (selectedObj)
			{
				// Remove the currently selected game object
				if (me.GetType() == MouseEvent::EventType::MPress)
				{
					_children.erase(std::remove(_children.begin(), _children.end(), selectedObj), _children.end());
					delete selectedObj;
					selectedObj = nullptr;
				}
			}
			break;
		case MouseEvent::EventType::RPress:
			if (!selectedObj)
			{
				// Create a new tile
				for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
				{
					if (object->GetTransform()->GetPosition().x == relPos.x && object->GetTransform()->GetPosition().y == relPos.y)
						return;
				}

				json tempJson = json::parse(_prefabs[_prefabNum].jsonString);
				tempJson[JSON_GO_NAME] = _prefabs[_prefabNum].name + " [" + std::to_string(_objNum) + "]";
				tempJson[JSON_GO_POSITION].push_back(float(relPos.x));
				tempJson[JSON_GO_POSITION].push_back(float(relPos.y));
				tempJson[JSON_GO_POSITION].push_back(0.0f);
				GameObject* tempObj = new GameObject(tempJson);
				_objNum++;

				_children.push_back(tempObj);
			}
			break;
		case MouseEvent::EventType::WheelUp:
			if (!selectedObj)
			{
				// Change tile type to be made
				_prefabNum = (_prefabNum + 1) % _prefabs.size();
				_texture = ObjectHandler::GetInstance().LoadDDSTextureFile(_prefabs[_prefabNum].ghostImageFilepath);
			}
			break;
		case MouseEvent::EventType::WheelDown:
			if (!selectedObj)
			{
				// Change tile type to be made
				_prefabNum = (_prefabNum - 1);
				// Because -1 % size() returns 0 (due to size being unsigned) and -1 % (int)size returns -1
				if (_prefabNum < 0)
					_prefabNum = _prefabs.size() - 1;
				_texture = ObjectHandler::GetInstance().LoadDDSTextureFile(_prefabs[_prefabNum].ghostImageFilepath);
			}
			break;
		}
	}
#else
	for (GameObject* obj : _children)
		obj->Update(deltaTime);
#endif
}

void Scene::FixedUpdate(float timeStep)
{
	
	for (GameObject* obj : _children)
	{
		obj->FixedUpdate(timeStep);
	}
}

void Scene::Stop()
{
	for (GameObject* obj : _children)
		obj->Stop();
}

void Scene::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	for (GameObject* object : _children)
		object->Render(context, constantBuffer, globalBuffer);
	_backgroundImage->Render(context, constantBuffer, globalBuffer);
#if EDIT_MODE
	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(_texture.width * _prefabs[_prefabNum].ghostTexCoords.x / 2, _texture.height * _prefabs[_prefabNum].ghostTexCoords.y / 2, 1.0f)
		* DirectX::XMMatrixTranslation(_ghost.x, _ghost.y, 0.0f);

	constantBuffer.mWorld = DirectX::XMMatrixTranspose(world);
	constantBuffer.mTexCoord = _prefabs[_prefabNum].ghostTexMatrix;
	constantBuffer.mAlphaMultiplier = 0.2f;

	context->UpdateSubresource(globalBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);

	// Draw object
	context->PSSetShaderResources(0, 1, &_texture.texture);
	context->IASetVertexBuffers(0, 1, _geometry.vertexBuffer.GetAddressOf(), &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	context->IASetIndexBuffer(_geometry.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_geometry.numOfIndices, 0, 0);
#endif
}
