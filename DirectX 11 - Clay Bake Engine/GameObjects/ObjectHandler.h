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

	ID3D11ShaderResourceView* GetLoadedTexture(std::string name) { return _loadedTextures[name]; }
	void AddTextureToMap(std::string name, ID3D11ShaderResourceView* texture) { _loadedTextures.emplace(name, texture); }

	Geometry GetSquareGeometry() { return _squareGeometry; }
	void SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride);

	static ObjectHandler* Instance()
	{
		static ObjectHandler instance;
		return &instance;
	}
private:
	std::unordered_map<std::string, GameObject*> _gameObjects = {};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> _loadedTextures = {};

	Geometry _squareGeometry;
};

