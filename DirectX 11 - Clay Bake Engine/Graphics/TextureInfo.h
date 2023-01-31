#pragma once

#include <d3d11.h>

struct TextureInfo
{
	ID3D11ShaderResourceView* texture;
	UINT width;
	UINT height;
};