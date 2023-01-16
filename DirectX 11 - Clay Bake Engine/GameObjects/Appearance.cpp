#include "Appearance.h"

Appearance::Appearance()
{
	_pTextureRV = nullptr;
}

void Appearance::SetTexCoords(float width, float height, float x, float y)
{
	//Width and height gives an area of the texture
	//x and y are the starting coordinates of the area
	//DirectX UV coordinates are normally 0,0 in the top left and 1,1 in the bottom right
	_texMatrix =
	{
		width, 0.0f, 0.0f, x,
		0.0f, height, 0.0f, y,
		0.0f, 0.0f, 0.0f, 0.0f,
		x, y, 0.0f, 0.0f
	};
	_texCoords = { width, height, x, y };
}

void Appearance::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context)
{
	// Draw object
	context->PSSetShaderResources(0, 1, _pTextureRV.GetAddressOf());
	context->IASetVertexBuffers(0, 1, _geometry.vertexBuffer.GetAddressOf(), &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	context->IASetIndexBuffer(_geometry.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_geometry.numOfIndices, 0, 0);
}
