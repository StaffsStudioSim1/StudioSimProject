#pragma once

#include <d3d11.h>

struct TextureInfo
{
	std::string filePath;
	ID3D11ShaderResourceView* texture;
	UINT width;
	UINT height;
};