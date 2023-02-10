#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->_shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"failed to load shader: "; errorMsg += shaderpath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateVertexShader(this->_shaderBuffer.Get()->GetBufferPointer(), this->_shaderBuffer.Get()->GetBufferSize(), NULL, this->_shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to Create Vertex Shader: "; errorMsg += shaderpath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numElements, this->_shaderBuffer->GetBufferPointer(), this->_shaderBuffer->GetBufferSize(), this->_inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create input layout. \n");
		return false;
	}
	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->_shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->_shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->_inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->_shaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += shaderpath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(this->_shaderBuffer.Get()->GetBufferPointer(), this->_shaderBuffer.Get()->GetBufferSize(), NULL, this->_shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create pixel shader: ";
		errorMsg += shaderpath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	return true;
}


ID3D11PixelShader* PixelShader::GetShader()
{
	return this->_shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->_shaderBuffer.Get();
}