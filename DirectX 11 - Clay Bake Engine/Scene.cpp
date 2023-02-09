#include "Scene.h"
#include <fstream>
#include "ErrorLogger.h"
#include "nlohmann/json.hpp"
#include "GameObjects/ObjectHandler.h"
using json = nlohmann::json;

#if EDIT_MODE
#include "Input/InputManager.h"
#endif
#include "GameObjects/Components/Rigidbody.h"
#include "Graphics/Graphics.h"

Scene::Scene(std::string filePath, int width, int height)
{
	_filePath = filePath;
	_width = width;
	_height = height;
	_windowScaleX = _width / INTERNAL_RESOLUTION_X;
	_windowScaleY = _height / INTERNAL_RESOLUTION_Y;

	std::ifstream f(filePath);
	if (!f.good())
		ErrorLogger::Log("Unable to find scene file " + filePath);

	json data = json::parse(f);

	_id = data[JSON_SCENE_ID];
	std::string imagePath = data[JSON_SCENE_BACKGROUND];
	_backgroundImage = new GameObject((std::string)JSON_SCENE_BACKGROUND);
	_backgroundImage->GetTransform()->SetDepthPos(1.0f);
	_backgroundImage->AddComponent(new Appearance(imagePath));

	std::string audioFilePath = data[JSON_SCENE_BACKGROUNDAUDIO];
	_backgroundAudio = new SoundEffect(audioFilePath, true);

	for (json objectData : data[JSON_SCENE_GAMEOBJECTS])
		_children.push_back(new GameObject(objectData));

	std::string map = data[JSON_SCENE_STAGECOLLISION];
#if !EDIT_MODE
	if (map != "")
	{
		// y, x
		int matrix[20][36];
		for (int i = 0; i < map.length(); i++)
		{
			matrix[i / 36][i % 36] = map[i] - 48;
		}
		for (int y = 0; y < 20; y++)
			for (int x = 0; x < 36; x++)
				if (matrix[y][x] == 1)
				{
					matrix[y][x] = 2;
					Vector2 rect = Vector2(1, 1);

					// Attempt to expand the box sideways
					for (int x2 = x + 1; x2 < 36; x2++)
					{
						if (matrix[y][x2] == 1)
						{
							rect.x++;
							matrix[y][x2] = 2;
						}
						else
							break;
					}

					// If that failed, try and expand upwards
					if (rect.x == 1)
						for (int y2 = y + 1; y2 < 20; y2++)
						{
							if (matrix[y2][x] == 1)
							{
								rect.y++;
								matrix[y2][x] = 2;
							}
							else
								break;
						}


					GameObject* go = new GameObject("Stage Collision (" + std::to_string(x) + "," + std::to_string(y) + ")");
					go->SetTag(JSON_TAG_STAGECOLLISION);

					go->GetTransform()->SetPosition(GridToWorld(Vector2(x, y)) + (rect - Vector2(1, 1)) * _snapScale / 2.0f);
					go->AddComponent(new AABB(_snapScale * rect.x, _snapScale * rect.y));

					_children.push_back(go);
				}
	}
#else
	/*RECT rc;
	GetWindowRect(GetActiveWindow(), &rc);
	int width = (rc.right - rc.left) - 16;
	int height = (rc.bottom - rc.top) - 39;*/
	_geometry = ObjectHandler::GetInstance().GetSquareGeometry();

	_prefabs.push_back(Prefab("Collision",		"Resources/Sprites/StageCollision.dds", { 1.0f, 1.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"tag\" : \"StageCollision\", \"scale\" : [1.0, 1.0], \"components\" : [ { \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/StageCollision.dds\", 1.0, 1.0, 0.0, 0.0, 1.0] } ] }"));
	_prefabs.push_back(Prefab("Player Blue",	"Resources/Sprites/Player1.dds", { 6.0f, 8.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.75, 1.75] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Player1.dds\", 6.0, 8.0, 0.0, 0.0, 1.0] }, { \"class\" : \"PlayerController\", \"constructors\" : [1] }, { \"class\" : \"AABB\", \"constructors\": [ 11, 16 ] }, { \"class\" : \"Rigidbody\" }] }"));
	_prefabs.push_back(Prefab("Player Red",		"Resources/Sprites/Player2.dds", { 6.0f, 8.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.75, 1.75] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Player2.dds\", 6.0, 8.0, 0.0, 0.0, 1.0] }, { \"class\" : \"PlayerController\", \"constructors\" : [2] }, { \"class\" : \"AABB\", \"constructors\": [ 11, 16 ] }, { \"class\" : \"Rigidbody\" }] }"));
	_prefabs.push_back(Prefab("Box",			"Resources/Sprites/Box.dds", { 1.0f, 1.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Box.dds\", 1.0, 1.0, 0.0, 0.0, 1.0] }] }"));
	_prefabs.push_back(Prefab("Lamp",			"Resources/Sprites/Lamp.dds", { 2.0f, 1.0f, 0.0f, 0.0f }, "{ \"rotation\" : 0.0, \"scale\" : [1.0, 1.0] , \"components\" : [{ \"class\" : \"Appearance\", \"constructors\" : [\"Resources/Sprites/Lamp.dds\", 2.0, 1.0, 0.0, 0.0, 1.0, 0.0, 18.0] }] }"));
	_prefabs.push_back(Prefab("Lever",			"Resources/Sprites/Lever.dds", { 3.0f, 1.0f, 0.0f, 0.0f }, "{ \"components\": [ { \"class\": \"Appearance\", \"constructors\": [ \"Resources/Sprites/Lever.dds\", 3.0, 1.0, 0.0, 0.0, 1.0 ] }, { \"class\": \"LeverComponent\", \"constructors\": [ 0, \"\" ] }, { \"class\": \"AABB\", \"constructors\": [ 18.0, 5.0 ] } ], \"rotation\": 0.0, \"scale\": [ 1.0, 1.0 ], \"tag\": \"GameObject\" }"));
	_prefabs.push_back(Prefab("Pressure Plate",	"Resources/Sprites/PressurePlate.dds", { 1.0f, 1.0f, 0.0f, 0.0f }, "{ \"components\": [ { \"class\": \"Appearance\", \"constructors\": [ \"Resources/Sprites/PressurePlate.dds\", 1.0, 1.0, 0.0, 0.0, 1.0 ] }, { \"class\": \"PressurePlateComponent\", \"constructors\": [ 0, \"\" ] }, { \"class\": \"AABB\", \"constructors\": [ 18.0, 5.0, true ] } ], \"rotation\": 0.0, \"scale\": [ 1.0, 1.0 ], \"tag\": \"GameObject\" }"));
	_prefabs.push_back(Prefab("Door",			"Resources/Sprites/Door.dds", { 1.0f, 1.0f, 0.0f, 0.0f }, "{ \"components\": [ { \"class\": \"Appearance\", \"constructors\": [ \"Resources/Sprites/Door.dds\", 1.0, 1.0, 0.0, 0.0, 1.0 ] }, { \"class\": \"DoorComponent\" }, { \"class\": \"AABB\", \"constructors\": [ 16.0, 32.0 ] } ], \"rotation\": 0.0, \"scale\": [ 1.0, 1.0 ], \"tag\": \"GameObject\" }"));
	_prefabs.push_back(Prefab("Goal",			"Resources/Sprites/Gold Goal.dds", { 1.0f, 6.0f, 0.0f, 0.0f }, "{ \"components\": [ { \"class\": \"Appearance\", \"constructors\": [ \"Resources/Sprites/Gold Goal.dds\", 1.0, 6.0, 0.0, 0.0, 1.0 ] }, { \"class\": \"Goal\", \"constructors\": [ \"\" ]}, { \"class\": \"AABB\", \"constructors\": [ 36.0, 36.0, true ] } ], \"rotation\": 0.0, \"scale\": [ 0.25, 0.25 ], \"tag\": \"GameObject\" }"));

	_texture = ObjectHandler::GetInstance().LoadDDSTextureFile(_prefabs[_prefabNum].ghostImageFilepath);

	if (map != "")
	{
		for (int x = 0; x < 36; x++)
			for (int y = 0; y < 20; y++)
				if (map[x + y * 36] == '1')
				{
					json tempJson = json::parse(_prefabs[0].jsonString);
					tempJson[JSON_GO_NAME] = _prefabs[0].name + " [" + std::to_string(_children.size()) + "]";
					Vector2 worldPos = GridToWorld(Vector2(x, y));
					tempJson[JSON_GO_POSITION].push_back(worldPos.x);
					tempJson[JSON_GO_POSITION].push_back(worldPos.y);
					tempJson[JSON_GO_POSITION].push_back(0.0f);
					GameObject* tempObj = new GameObject(tempJson);
					_children.push_back(tempObj);
					}

		_objNum = _children.size();
				}
#endif
	}

Scene::~Scene()
{
	for (GameObject* obj : _children)
		delete obj;
	_children.clear();

	delete _backgroundImage;
	delete _backgroundAudio;
}

void Scene::Save()
{
#if EDIT_MODE
	json scene;
	json gameObjects;

	scene[JSON_SCENE_ID] = _id;
	scene[JSON_SCENE_BACKGROUND] = ObjectHandler::GetInstance().GetGameObject(0)->GetComponent<Appearance>()->GetTexture().filePath; // Presumes that the first object is the background
	scene[JSON_SCENE_BACKGROUNDAUDIO] = _audioFilePath;

	std::string map;
	for (int i = 0; i < 36 * 20; i++)
		map += '0';

	for (GameObject* obj : _children)
	{
		if (obj->GetTag() == JSON_TAG_GAMEOBJECT)
			gameObjects.push_back(obj->Write());
		else if (obj->GetTag() == JSON_TAG_STAGECOLLISION)
		{
			Vector2 gridPos = WorldToGrid(obj->GetTransform()->GetPosition());
			// For each 1 y, 36 x's have to be added
			map = map.replace(gridPos.x + gridPos.y * 36, 1, "1");
}
	}

	scene[JSON_SCENE_GAMEOBJECTS] = gameObjects;
	scene[JSON_SCENE_STAGECOLLISION] = map;

	std::ofstream o(_filePath);
	o << std::setw(4) << scene << std::endl;
	o.close();
#endif
}

int Scene::GetID()
{
	return _id;
}

void Scene::Start()
{
	for (GameObject* obj : _children)
		obj->Start();
#if !EDIT_MODE
	_backgroundAudio->Play();
#endif
}

void Scene::Update(float deltaTime)
{
#if EDIT_MODE
	static GameObject* selectedObj = nullptr;
	static Vector2 startingPos = { 0, 0 };

	MouseClass* mouse = InputManager::GetInstance().GetMouse();
	Vector2 mousePos = Vector2(mouse->GetPosX(), mouse->GetPosY());

	Vector2 relPos = GetRelativeMousePos(mousePos);
	relPos = SnapCoordinatesToGrid(relPos);
	_ghost = relPos;

	if (selectedObj)
	{
		// If an object is following the cursor
		Vector2 relativeMousePos = GetRelativeMousePos(mousePos);
		selectedObj->GetTransform()->SetPosition(relativeMousePos);
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
				selectedObj = TestForObjectIntersection(mousePos);
				if (selectedObj)
					startingPos = selectedObj->GetTransform()->GetPosition();
			}
			break;
		case MouseEvent::EventType::LRelease:
			if (selectedObj)
			{
				// Drop tile at position
				Vector2 objectPos = selectedObj->GetTransform()->GetPosition();
				Vector2 snapPos = SnapCoordinatesToGrid(objectPos);

				for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
				{
					if (object != selectedObj && object->GetTransform()->GetPosition() == snapPos)
					{
						selectedObj->GetTransform()->SetPosition(startingPos);
						selectedObj = nullptr;
						return;
					}
				}

				selectedObj->GetTransform()->SetPosition(snapPos);
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
					if (object->GetTransform()->GetPosition() == relPos)
						return;

				Vector2 relPos = GetRelativeMousePos(mousePos);
				relPos = SnapCoordinatesToGrid(relPos);

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
#if !EDIT_MODE
	for (GameObject* obj : _children)
		obj->FixedUpdate(timeStep);

	for (int i = 0; i < _children.size(); i++)
	{
		if (!_children[i]->HasRigidbody())
			continue;

		Rigidbody* rbI = _children[i]->GetComponent<Rigidbody>();
		for (int j = 0; j < _children.size(); j++)
		{
			if (!_children[j]->HasCollider() || _children[j]->HasRigidbody())
				continue;

			if (_children[i]->GetComponent<AABB>()->Overlaps(_children[j]->GetComponent<AABB>(), timeStep))
				rbI->Collide(_children[j]);
		}
	}
#endif
}

void Scene::Stop()
{
	for (GameObject* obj : _children)
		obj->Stop();
	_backgroundAudio->Stop();
}

void Scene::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	for (GameObject* object : _children)
		object->Render(context, constantBuffer, globalBuffer);
	_backgroundImage->Render(context, constantBuffer, globalBuffer);
#if EDIT_MODE
	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(_texture.width * _prefabs[_prefabNum].ghostTexCoords.x / 2, _texture.height * _prefabs[_prefabNum].ghostTexCoords.y / 2, 1.0f)
		* DirectX::XMMatrixTranslation(_ghost.x + _prefabs[_prefabNum].ghostOffset.x, _ghost.y + _prefabs[_prefabNum].ghostOffset.y, 0.0f);

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

GameObject* Scene::TestForObjectIntersection(Vector2 mousePos)
{
	Vector2 gridPos = SnapCoordinatesToGrid(GetRelativeMousePos(mousePos));

	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		if (object->GetName() == JSON_SCENE_BACKGROUND)
			continue;

		if (object->GetTransform()->GetPosition() == gridPos)
			return object;
	}

	return nullptr;
}

Vector2 Scene::GetRelativeMousePos(Vector2 mousePos)
{
	return Vector2((mousePos.x - (_width / 2)) / _windowScaleX, -(mousePos.y - (_height / 2)) / _windowScaleY);
}

Vector2 Scene::SnapCoordinatesToGrid(Vector2 worldPos)
{
	return GridToWorld(WorldToGrid(worldPos));
}

Vector2 Scene::WorldToGrid(Vector2 worldPos)
{
	int x = (worldPos.x + INTERNAL_RESOLUTION_X / 2) / _snapScale;
	int y = (worldPos.y + INTERNAL_RESOLUTION_Y / 2) / _snapScale;
	return Vector2(x, y);
}

Vector2 Scene::GridToWorld(Vector2 gridPos)
{
	gridPos *= _snapScale;
	gridPos += Vector2(_snapScale / 2, _snapScale / 2);
	gridPos -= Vector2(INTERNAL_RESOLUTION_X / 2, INTERNAL_RESOLUTION_Y / 2);
	return gridPos;
}
