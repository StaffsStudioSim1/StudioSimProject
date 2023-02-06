#pragma once

#include <DDSTextureLoader.h>
#include <unordered_map>
#include "Appearance.h"
#include "GameObject.h"
#include "../Graphics/TextureInfo.h"
#include "Physics.h"

class ObjectHandler
{
public:
	// Singleton Code
	static ObjectHandler& GetInstance()
	{
		static ObjectHandler instance;
		return instance;
	}

	ObjectHandler(ObjectHandler const&) = delete;
	void operator=(ObjectHandler const&) = delete;

	void Initialise(Microsoft::WRL::ComPtr <ID3D11Device> device);

	std::vector<GameObject*> GetAllObjects() { return _gameObjects; }
	GameObject* GetGameObject(int objNum) { return _gameObjects[objNum]; }
	GameObject* FindGameObject(std::string name);
	GameObject* FindGameObject(int id);

	int SetObjectID();

	void Register(GameObject* object);
	void Unregister(GameObject* object);

	TextureInfo LoadDDSTextureFile(std::string filePath, bool changeTexture = false);
	void ClearLoadedTextures();

	Geometry GetSquareGeometry() { return _squareGeometry; }
	void SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride);
	
	PhysicsWorld* GetPhysicsWorld() { return _pPhysicsWorld; }
	void SetPhysicsWorld(PhysicsWorld* physics) { _pPhysicsWorld = physics; }

	// Toggle UI menu functions
	bool IsMainMenu() { return _mainMenu;}
	void SetMainMenu(bool menu) { _mainMenu = menu; }

	bool IsLevelSelect() { return _levelSelect; }
	void SetLevelSelect(bool levelSelect) { _levelSelect = levelSelect; }

	bool IsOptionsMenu() { return _optionsMenu; }
	void SetOptionsMenu(bool optionsMenu) { _optionsMenu = optionsMenu; }

	bool IsPauseMenu() { return _pauseMenu; }
	void SetPauseMenu(bool pauseMenu) { _pauseMenu = pauseMenu; }
private:
	ObjectHandler();
	~ObjectHandler();

	bool _initialised = false;
	int _objectID = 0;

	bool _mainMenu = true;
	bool _levelSelect = false;
	bool _optionsMenu = false;
	bool _pauseMenu = false;

	Microsoft::WRL::ComPtr <ID3D11Device>	_device;

	std::vector<GameObject*> _gameObjects = {};

	std::unordered_map<std::string, TextureInfo> _loadedTextures = {};

	Geometry _squareGeometry;

	PhysicsWorld* _pPhysicsWorld;
};

