#pragma once
#include <vector>
#include <string>
#include "GameObjects/GameObject.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/TextureInfo.h"

#define EDIT_MODE true

#if EDIT_MODE
#include "Graphics/Geometry.h"
#include "Input/Mouse/MousePicking.h"
#include "GameObjects/ObjectHandler.h"

struct Prefab
{
	std::string name;
	std::string ghostImageFilepath;
	DirectX::XMFLOAT4 ghostTexCoords;
	DirectX::XMFLOAT4X4 ghostTexMatrix;
	std::string jsonString;

	Prefab(std::string name, std::string ghostImageFilepath, DirectX::XMFLOAT4 texCoords, std::string jsonString)
	{
		this->name = name;
		this->ghostImageFilepath = ghostImageFilepath;

		TextureInfo tex = ObjectHandler::GetInstance().LoadDDSTextureFile(ghostImageFilepath);
		float width = 1.0f / texCoords.x;
		float height = 1.0f / texCoords.y;
		float x = width * texCoords.z;
		float y = height * texCoords.w;

		ghostTexMatrix =
		{
			width, 0.0f, 0.0f, x,
			0.0f, height, 0.0f, y,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};
		ghostTexCoords = { width, height, x, y };

		this->jsonString = jsonString;
	}
};
#endif

class Scene
{
public:
	Scene(std::string filePath);
	~Scene();

	void Save();
	int GetID();

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);

	std::string GetFilePath() const { return _filePath; }
private:
	int _id;
	std::vector<GameObject*> _children;
	GameObject* _backgroundImage;
	std::string _filePath;

#if EDIT_MODE
	MousePicking _mousePicking = {};
	TextureInfo _texture;
	Geometry _geometry;
	Vector2 _ghost;
	int _objNum = 0;
	std::vector<Prefab> _prefabs;
	int _prefabNum = 0;

	//bool _collision[36][20];
#endif
};

