#pragma once
#include <vector>
#include <string>
#include "GameObjects/GameObject.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/TextureInfo.h"
#include "Defines.h"
#include "Audio/SoundEffect.h"

#if EDIT_MODE
#include "Graphics/Geometry.h"
#include "GameObjects/ObjectHandler.h"


struct Prefab
{
	std::string name;
	std::string ghostImageFilepath;
	DirectX::XMFLOAT4 ghostTexCoords;
	DirectX::XMFLOAT4X4 ghostTexMatrix;
	Vector2 ghostOffset;
	std::string jsonString;

	Prefab(std::string name, std::string ghostImageFilepath, DirectX::XMFLOAT4 texCoords, std::string jsonString, Vector2 ghostOffset = Vector2())
	{
		this->name = name;
		this->ghostImageFilepath = ghostImageFilepath;
		this->ghostOffset = ghostOffset;

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
	Scene(std::string filePath, int width, int height);
	~Scene();

	void Save();
	int GetID();

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);

	std::string GetFilePath() const { return _filePath; }
#if EDIT_MODE
	void SetFileName(std::string fileName) { _filePath = fileName; }
#endif

private:
	int _id;
	std::vector<GameObject*> _children;
	GameObject* _backgroundImage;
	std::string _filePath;
	SoundEffect* _backgroundAudio;
	std::string _audioFilePath;
	int _width;
	int _height;
	float _snapScale = 18;
	float _windowScaleX;
	float _windowScaleY;

#if EDIT_MODE
	TextureInfo _texture;
	Geometry _geometry;
	Vector2 _ghost;
	int _objNum = 0;
	std::vector<Prefab> _prefabs;
	int _prefabNum = 0;

	//bool _collision[36][20];
#endif

	GameObject* TestForObjectIntersection(Vector2 mousePos);
	Vector2 GetRelativeMousePos(Vector2 mousePos);
	Vector2 SnapCoordinatesToGrid(Vector2 worldPos);
	Vector2 WorldToGrid(Vector2 worldPos);
	Vector2 GridToWorld(Vector2 gridPos);
};

