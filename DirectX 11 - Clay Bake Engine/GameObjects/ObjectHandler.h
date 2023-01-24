#pragma once

#include <DDSTextureLoader.h>
#include <unordered_map>
#include "Appearance.h"
#include "GameObject.h"
#include "TextureInfo.h"

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

	void Register(GameObject* object);
	void Unregister(GameObject* object);

	TextureInfo LoadDDSTextureFile(std::string filePath);
	void ClearLoadedTextures();

	Geometry GetSquareGeometry() { return _squareGeometry; }
	void SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride);
private:
	ObjectHandler();
	~ObjectHandler();

	bool _initialised = false;

	Microsoft::WRL::ComPtr <ID3D11Device>	_device;

	std::vector<GameObject*> _gameObjects = {};

	std::unordered_map<std::string, TextureInfo> _loadedTextures = {};

	Geometry _squareGeometry;
};

