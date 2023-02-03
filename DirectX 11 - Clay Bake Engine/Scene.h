#pragma once
#include <vector>
#include <string>
#include "GameObjects/GameObject.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/TextureInfo.h"

#define EDIT_MODE false

#if EDIT_MODE
#include "Graphics/Geometry.h"
#include "Input/Mouse/MousePicking.h"
#endif

class Scene
{
public:
	Scene(std::string filePath);
	~Scene();

	void Save();

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);

#if EDIT_MODE
	void SetFileName(std::string fileName) { _fileName = fileName; }
	std::string GetFileName() const { return _fileName; }
#endif
private:
	std::vector<GameObject*> _children;
	GameObject* _backgroundImage;

#if EDIT_MODE
	MousePicking _mousePicking = {};
	TextureInfo _texture;
	Geometry _geometry;
	Vector2 _ghost;
	std::string _fileName = "";
	std::vector<std::string> _textureNames = { "temp_tile.dds", "Test.dds", "Test2.dds" };
	int _textureNum = 0;
#endif
};

