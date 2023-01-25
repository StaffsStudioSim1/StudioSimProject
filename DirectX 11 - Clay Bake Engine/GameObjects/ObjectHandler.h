#pragma once

#include <unordered_map>

#include "GameObject.h"

class ObjectHandler
{
public:
	ObjectHandler();
	~ObjectHandler() {}

	GameObject* GetGameObject(std::string name) { return _gameObjects[name]; }
	std::unordered_map<std::string, GameObject*> GetAllObjects() { return _gameObjects; }
	void AddGameObjectToMap(std::string name, GameObject* object) { _gameObjects.emplace(name, object); }
	void CreateGameObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 scale, float rotation, bool hasPhysics = false, std::string textureName = "", DirectX::XMFLOAT4 texCoords = { 1.0f, 1.0f, 0.0f, 0.0f }, float alphaMul = 1.0f);
	void RemoveGameObject(std::string name);
	void ClearGameObjects();

	void Initialise(Microsoft::WRL::ComPtr <ID3D11Device> device);

	std::vector<GameObject*> GetAllObjects() { return _gameObjects; }

	void Register(GameObject* object);
	void Unregister(GameObject* object);

	TextureInfo LoadDDSTextureFile(std::string filePath);
	void ClearLoadedTextures();

	Geometry GetSquareGeometry() { return _squareGeometry; }
	void SetSquareGeometry(Geometry square) { _squareGeometry = square; }
private:
	std::unordered_map<std::string, GameObject*> _gameObjects = {};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> _loadedTextures = {};

	Geometry _squareGeometry;
};

