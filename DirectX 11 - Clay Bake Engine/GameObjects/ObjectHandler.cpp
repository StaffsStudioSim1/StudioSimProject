#include "ObjectHandler.h"
#include "../ErrorLogger.h"
#include <DirectXCollision.h>
#include "Components/AABB.h"

ObjectHandler::ObjectHandler()
{

}

ObjectHandler::~ObjectHandler()
{
	ClearLoadedTextures();
}

void ObjectHandler::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	if (_initialised)
		return;
	_initialised = true;
	_device = device;
}

GameObject* ObjectHandler::FindGameObject(std::string name)
{
	for (GameObject* object : _gameObjects)
		if (object->GetName() == name)
			return object;
	return nullptr;
}

GameObject* ObjectHandler::FindGameObject(int id)
{
	for (GameObject* object : _gameObjects)
	{
		if (object->GetID() == id)
			return object;
	}
	return nullptr;
}

int ObjectHandler::SetObjectID()
{
	_objectID++;
	return _objectID - 1;
}

void ObjectHandler::Register(GameObject* object)
{
	_gameObjects.push_back(object);
}

void ObjectHandler::Unregister(GameObject* object)
{
	_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), object), _gameObjects.end());
}

TextureInfo ObjectHandler::LoadDDSTextureFile(std::string filePath, bool changeTexture)
{
	if (!_initialised)
		return TextureInfo();

	HRESULT hr;

	std::unordered_map<std::string, TextureInfo>::iterator it = _loadedTextures.find(filePath);
	if (it != _loadedTextures.end())
		return it->second;

	// Load texture from file
	ID3D11ShaderResourceView* tempTexture = nullptr;
	wchar_t wideFilePath[256];
	mbstowcs_s(nullptr, wideFilePath, filePath.c_str(), _TRUNCATE);

	Microsoft::WRL::ComPtr<ID3D11Resource> res;

	hr = DirectX::CreateDDSTextureFromFile(_device.Get(), wideFilePath, res.GetAddressOf(), &tempTexture);
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to load DDS Texture!\nFile path: " + filePath + "\nTexture name: " + filePath);
		if (changeTexture) // When in edit mode and texture changing fails return an empty TextureInfo instead of crashing
			return TextureInfo();
		exit(EXIT_FAILURE);
	}

	// Get data from loaded texture
	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	res->GetType(&resType);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	hr = res.As(&tex);
	if (FAILED(hr))
	{
		ErrorLogger::Log("Failed to get DDS Texture information\n");
		exit(EXIT_FAILURE);
	}
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	TextureInfo textureInfo = { filePath, tempTexture, desc.Width, desc.Height };

	_loadedTextures.emplace(filePath, textureInfo);
	return textureInfo;
}

void ObjectHandler::ClearLoadedTextures()
{
	for (std::pair<std::string, TextureInfo> texture : _loadedTextures)
	{
		texture.second.texture->Release();
		texture.second.texture = nullptr;
	}
	_loadedTextures.clear();
}

void ObjectHandler::SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride)
{
	_squareGeometry.vertexBuffer = vertexBuffer;
	_squareGeometry.indexBuffer = indexBuffer;
	_squareGeometry.numOfIndices = numOfIndices;
	_squareGeometry.vertexBufferOffset = vertexBufferOffset;
	_squareGeometry.vertexBufferStride = vertexBufferStride;
}

std::vector<GameObject*> ObjectHandler::GetObjectsInArea(Vector2 position, Vector2 size)
{
	std::vector<GameObject*> objects;
	DirectX::BoundingBox box1;
	box1.Center = { position.x,position.y, 0.0f };
	box1.Extents = { size.x / 2, size.y / 2, 0.0f };


	DirectX::BoundingBox box2;
	for (GameObject* object : GetAllObjects())
	{
		if (object->GetTag() != JSON_TAG_GAMEOBJECT || !object->HasCollider())
			continue;

		AABB* aabb = object->GetComponent<AABB>();
		Vector2 checkPos = object->GetTransform()->GetPosition();
		Vector2 checkSize = aabb->GetSize();
		box2.Center = { checkPos.x, checkPos.y, 0.0f };
		box2.Extents = { checkSize.x / 2, checkSize.y / 2, 0.0f };

		if (box1.Intersects(box2))
		{
			objects.push_back(object);
		}
	}

	return objects;
}

std::vector<GameObject*> ObjectHandler::GetStageCollisionInArea(Vector2 position, Vector2 size)
{
	std::vector<GameObject*> objects;
	DirectX::BoundingBox box1;
	box1.Center = { position.x,position.y, 0.0f };
	box1.Extents = { size.x / 2, size.y / 2, 0.0f };


	DirectX::BoundingBox box2;
	for (GameObject* object : GetAllObjects())
	{
		if (object->GetTag() != JSON_TAG_STAGECOLLISION)
			continue;

		AABB* aabb = object->GetComponent<AABB>();
		Vector2 checkPos = object->GetTransform()->GetPosition();
		Vector2 checkSize = aabb->GetSize();
		box2.Center = { checkPos.x, checkPos.y, 0.0f };
		box2.Extents = { checkSize.x / 2, checkSize.y / 2, 0.0f };

		if (box1.Intersects(box2))
		{
			objects.push_back(object);
		}
	}
	OutputDebugStringA(std::to_string(objects.size()).c_str());
	if (objects.size() > 0)
		OutputDebugStringA(objects.at(0)->GetName().c_str());
	OutputDebugStringA("\n");
	return objects;
}
