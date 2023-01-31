#include "ObjectHandler.h"
#include "../ErrorLogger.h"

ObjectHandler::ObjectHandler()
{

}

ObjectHandler::~ObjectHandler()
{
	ClearLoadedTextures();
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

TextureInfo ObjectHandler::LoadDDSTextureFile(std::string filePath)
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
