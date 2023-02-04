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
	_backgroundImage = new GameObject((std::string) JSON_SCENE_BACKGROUND);
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

	_prefabs.push_back(Prefab("Resources/Sprites/Player1.dds", {6.0f, 8.0f, 0.0f, 0.0f}, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Player1.dds\", 6.0, 8.0, 0.0, 0.0, 1.0] }, { \"class\" : \"PlayerController\", \"constructors\" : [1] }] }"));
	_prefabs.push_back(Prefab("Resources/Sprites/Player2.dds", {6.0f, 8.0f, 0.0f, 0.0f}, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Player2.dds\", 6.0, 8.0, 0.0, 0.0, 1.0] }, { \"class\" : \"PlayerController\", \"constructors\" : [2] }] }"));
	_prefabs.push_back(Prefab("Resources/Sprites/Box.dds", {1.0f, 1.0f, 0.0f, 0.0f}, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Box.dds\", 1.0, 1.0, 0.0, 0.0, 1.0] }] }"));
	_prefabs.push_back(Prefab("Resources/Sprites/Lamp.dds", {2.0f, 1.0f, 0.0f, 0.0f}, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Lamp.dds\", 2.0, 1.0, 0.0, 0.0, 1.0] }] }"));
	
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

	scene[JSON_SCENE_BACKGROUND] = "Resources/Textures/ZoeLevel.dds";

	for (GameObject* obj : _children)
	{
		gameObjects.push_back(obj->Write());
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

	while (!mouse->EventBufferIsEmpty()) // Handles moving, creating and deleting objects with the mouse in edit mode
	{
		MouseEvent me = mouse->ReadEvent();
		// Pickup Tile
		if (me.GetType() == MouseEvent::EventType::LPress && !selectedObj) // Probably can be changed to a switch statement
		{
			selectedObj = _mousePicking.TestForObjectIntersection(mousePos.x, mousePos.y);
			if (selectedObj)
			{
				Vector2 pos = selectedObj->GetTransform()->GetPosition();
				startingPos = { (int)pos.x, (int)pos.y };
			}
		}
		// Drop tile at position
		else if (me.GetType() == MouseEvent::EventType::LRelease && selectedObj)
		{
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
		// If an object is following the cursor
		else if (selectedObj)
		{
			DirectX::XMINT2 relativeMousePos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
			selectedObj->GetTransform()->SetPosition(relativeMousePos.x, relativeMousePos.y);

			if (me.GetType() == MouseEvent::EventType::MPress) // Remove the currently selected game object
			{
				_children.erase(std::remove(_children.begin(), _children.end(), selectedObj), _children.end());
				delete selectedObj;
				selectedObj = nullptr;
			}
		}
		// Create a new tile
		else if (me.GetType() == MouseEvent::EventType::RPress && !selectedObj) // Creates a new game object
		{
			for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
			{
				if (object->GetTransform()->GetPosition().x == relPos.x && object->GetTransform()->GetPosition().y == relPos.y)
					return;
			}

			json tempJson = json::parse(_prefabs[_prefabNum].jsonString);
			tempJson[JSON_GO_NAME] = "Object" + std::to_string(_objNum);
			tempJson[JSON_GO_POSITION].push_back(float(relPos.x));
			tempJson[JSON_GO_POSITION].push_back(float(relPos.y));
			tempJson[JSON_GO_POSITION].push_back(0.0f);
			GameObject* tempObj = new GameObject(tempJson);
			_objNum++;

			_children.push_back(tempObj);
		}
		// Change tile type to be made
		else if (me.GetType() == MouseEvent::EventType::WheelUp && !selectedObj) // Changes the texture that the next game object will be created with
		{
			_prefabNum = (_prefabNum + 1) % _prefabs.size();
			_texture = ObjectHandler::GetInstance().LoadDDSTextureFile(_prefabs[_prefabNum].ghostImageFilepath);
		}
		// Change tile type to be made
		else if (me.GetType() == MouseEvent::EventType::WheelDown && !selectedObj)
		{
			_prefabNum = (_prefabNum - 1) % _prefabs.size();
			_texture = ObjectHandler::GetInstance().LoadDDSTextureFile(_prefabs[_prefabNum].ghostImageFilepath);
		}
	}
#else
	for (GameObject* obj : _children)
		obj->Update(deltaTime);
#endif
}

void Scene::FixedUpdate(float timeStep)
{
	ObjectHandler::GetInstance().GetPhysicsWorld()->world->Step(timeStep, 8, 3);
	for (GameObject* obj : _children)
		obj->FixedUpdate(timeStep);
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
		DirectX::XMMatrixScaling(_texture.width * _prefabs[_prefabNum].ghostTexCoords.x / 2,_texture.height * _prefabs[_prefabNum].ghostTexCoords.y / 2, 1.0f)
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
