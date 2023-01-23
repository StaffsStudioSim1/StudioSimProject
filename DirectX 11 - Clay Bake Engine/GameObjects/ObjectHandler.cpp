#include "ObjectHandler.h"
#include "../ErrorLogger.h"

ObjectHandler::ObjectHandler()
{

}

ObjectHandler::~ObjectHandler()
{

}

/*void ObjectHandler::CreateGameObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 scale, float rotation, bool hasPhysics)
{
	GameObject* tempObject = new GameObject(0);
	tempObject->AddComponent(new Appearance);
	tempObject->AddTransform(new Transform);

	// Set transformation values
	tempObject->GetTransform()->SetPosition(position);
	tempObject->GetTransform()->SetScale(scale);
	tempObject->GetTransform()->SetRotation(rotation);

	// Set appearance values
	tempObject->GetAppearance()->SetTexture(GetLoadedTexture(textureName));
	tempObject->GetAppearance()->SetTexCoords(texCoords);
	tempObject->GetAppearance()->SetAlphaMultiplier(alphaMul);
	tempObject->GetAppearance()->SetGeometryData(GetSquareGeometry());

	// Add physics if needed
	if (hasPhysics)
	{
		tempObject->AddPhysics(new Physics(tempObject->GetTransform()));
	}

	// Add to map
	RegisterObject(name, tempObject);
}*/

void ObjectHandler::Initialise(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	if (_initialised)
		return;
	_initialised = true;
	_device = device;
}

void ObjectHandler::Register(GameObject* object)
{
	_gameObjects.push_back(object);
}

void ObjectHandler::Unregister(GameObject* object)
{
	_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), object), _gameObjects.end());
}

ID3D11ShaderResourceView* ObjectHandler::LoadDDSTextureFile(std::string filePath)
{
	if (!_initialised)
		return nullptr;

	std::unordered_map<std::string, ID3D11ShaderResourceView*>::iterator it = _loadedTextures.find(filePath);
	if (it != _loadedTextures.end())
		return it->second;

	ID3D11ShaderResourceView* tempTexture = nullptr;
	wchar_t wideFilePath[256];
	mbstowcs_s(nullptr, wideFilePath, filePath.c_str(), _TRUNCATE);

	if (FAILED(DirectX::CreateDDSTextureFromFile(_device.Get(), wideFilePath, nullptr, &tempTexture)))
	{
		ErrorLogger::Log("Failed to load DDS Texture!\nFile path: " + filePath + "\nTexture name: " + filePath);
		exit(EXIT_FAILURE);
	}

	_loadedTextures.emplace(filePath, tempTexture);
	return tempTexture;
}

void ObjectHandler::SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride)
{
	_squareGeometry.vertexBuffer = vertexBuffer;
	_squareGeometry.indexBuffer = indexBuffer;
	_squareGeometry.numOfIndices = numOfIndices;
	_squareGeometry.vertexBufferOffset = vertexBufferOffset;
	_squareGeometry.vertexBufferStride = vertexBufferStride;
}
