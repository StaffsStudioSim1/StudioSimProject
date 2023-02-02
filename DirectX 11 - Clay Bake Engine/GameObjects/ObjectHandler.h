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

	void Register(GameObject* object);
	void Unregister(GameObject* object);

	TextureInfo LoadDDSTextureFile(std::string filePath);
	void ClearLoadedTextures();

	Geometry GetSquareGeometry() { return _squareGeometry; }
	void SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride);
	
	PhysicsWorld* GetPhysicsWorld() { return _pPhysicsWorld; }
	void SetPhysicsWorld(PhysicsWorld* physics) { _pPhysicsWorld = physics; }
private:
	ObjectHandler();
	~ObjectHandler();

	bool _initialised = false;

	Microsoft::WRL::ComPtr <ID3D11Device>	_device;

	std::vector<GameObject*> _gameObjects = {};

	std::unordered_map<std::string, TextureInfo> _loadedTextures = {};

	Geometry _squareGeometry;

	PhysicsWorld* _pPhysicsWorld;
};

