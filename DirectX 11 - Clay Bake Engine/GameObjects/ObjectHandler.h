#pragma once

#include <DDSTextureLoader.h>
#include <unordered_map>
#include "Components/Appearance.h"
#include "GameObject.h"
#include "../Graphics/TextureInfo.h"
#include "Components/Physics.h"

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

	bool IsMainMainUIEnabled() { return _mainMenuUI; }
	void EnableMainMenuUI(bool showUI) { _mainMenuUI = showUI; }

	bool IsLevelSelectUIEnabled() { return _levelSelectUI; }
	void EnableLevelSelectUI(bool showUI) { _levelSelectUI = showUI; }

	bool IsOptionsMenuUIEnabled() { return _optionsMenuUI; }
	void EnableOptionsMenuUI(bool showUI) { _optionsMenuUI = showUI; }

	bool IsPauseMenuUIEnabled() { return _pauseMenuUI; }
	void EnablePauseMenuUI(bool showUI) { _pauseMenuUI = showUI; }
private:
	ObjectHandler();
	~ObjectHandler();

	bool _initialised = false;
	int _objectID = 0;

	bool _mainMenuUI = true;
	bool _levelSelectUI = false;
	bool _optionsMenuUI = false;
	bool _pauseMenuUI = false;

	Microsoft::WRL::ComPtr <ID3D11Device>	_device;

	std::vector<GameObject*> _gameObjects = {};

	std::unordered_map<std::string, TextureInfo> _loadedTextures = {};

	Geometry _squareGeometry;

	PhysicsWorld* _pPhysicsWorld;
};

