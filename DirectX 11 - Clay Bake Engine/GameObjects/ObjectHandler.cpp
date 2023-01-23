#include "ObjectHandler.h"
#include "../ErrorLogger.h"

ObjectHandler::ObjectHandler()
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

void ObjectHandler::Unregister(GameObject* object)
{
	_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), object), _gameObjects.end());
}

void ObjectHandler::LoadDDSTextureFile(std::string filePath, std::string textureName, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	ID3D11ShaderResourceView* tempTexture = nullptr;
	wchar_t wideFilePath[256];
	mbstowcs_s(nullptr, wideFilePath, filePath.c_str(), _TRUNCATE);

	if (FAILED(DirectX::CreateDDSTextureFromFile(device.Get(), wideFilePath, nullptr, &tempTexture)))
	{
		ErrorLogger::Log("Failed to load DDS Texture!\nFile path: " + filePath + "\nTexture name: " + textureName);
		exit(EXIT_FAILURE);
	}

	_loadedTextures.emplace(textureName, tempTexture);
}

void ObjectHandler::SetSquareGeometry(Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer, UINT numOfIndices, UINT vertexBufferOffset, UINT vertexBufferStride)
{
	_squareGeometry.vertexBuffer = vertexBuffer;
	_squareGeometry.indexBuffer = indexBuffer;
	_squareGeometry.numOfIndices = numOfIndices;
	_squareGeometry.vertexBufferOffset = vertexBufferOffset;
	_squareGeometry.vertexBufferStride = vertexBufferStride;
}
