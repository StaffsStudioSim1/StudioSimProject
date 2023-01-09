#include "Appearance.h"

Appearance::Appearance()
{
	_pTextureRV = nullptr;
}

void Appearance::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context)
{
	// Draw object - would need texture uv stuff in shader
	//context->IASetVertexBuffers(0, 1, _geometry.vertexBuffer.GetAddressOf(), &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	//context->IASetIndexBuffer(_geometry.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	//context->DrawIndexed(_geometry.numOfIndices, 0, 0);

	//Add sprite batch rendering here?
}
