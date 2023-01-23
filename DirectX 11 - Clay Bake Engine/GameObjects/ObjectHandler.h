#pragma once

#include <unordered_map>

#include "GameObject.h"

class ObjectHandler
{
public:
	ObjectHandler();
	~ObjectHandler() {}

	std::vector<GameObject*> GetAllObjects() { return _gameObjects; }

	void Register(GameObject* object) { _gameObjects.push_back(object); }
	void Unregister(GameObject* object);

	ID3D11ShaderResourceView* GetLoadedTexture(std::string name) { return _loadedTextures[name]; }
	void AddTextureToMap(std::string name, ID3D11ShaderResourceView* texture) { _loadedTextures.emplace(name, texture); }

	Geometry GetSquareGeometry() { return _squareGeometry; }
	void SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride);

	static ObjectHandler* GetInstance()
	{
		static ObjectHandler instance;
		return &instance;
	}
private:
	std::vector<GameObject*> _gameObjects = {};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> _loadedTextures = {};

	Geometry _squareGeometry;
};

