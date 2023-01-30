#pragma once
#include <vector>
#include <string>
#include "GameObjects/GameObject.h"
#include "Graphics/ConstantBuffer.h"

#define EDIT_MODE false

#if EDIT_MODE
#include "Input/Mouse/MousePicking.h"
#endif

class Scene
{
public:
	Scene(std::string filePath);
	~Scene();

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();
	void SaveScene();


	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);
private:
	std::vector<GameObject*> _children;
	json data;


#if EDIT_MODE
	MousePicking _mousePicking = {};
	std::vector<std::string> _textureNames = { "Test.dds", "Test2.dds" };
	int _textureNum = 0;
#endif
};

