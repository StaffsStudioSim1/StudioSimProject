#pragma once
#include "Component.h"
#include "../../Physics/Vectors.h"
#include <DirectXCollision.h>
#include "../../Defines.h"

#if DEBUG
#include "../ObjectHandler.h"
#include "../../Graphics/Geometry.h"
#include "../../Graphics/TextureInfo.h"
#endif

class AABB :
	public Component
{
public:
	AABB(float width, float height, bool trigger = false);

	json Write();

	bool Overlaps(AABB* collider, float deltaTime);
	Vector2 GetSize();
	bool IsTrigger() { return _trigger; }

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);
private:
	float _width;
	float _height;
	bool _trigger;

#if DEBUG
	Geometry _debugGeo;
	TextureInfo _debugTex;
#endif
};

