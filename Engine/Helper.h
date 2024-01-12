#pragma once

namespace Engine
{
	std::wstring ToWString(const std::string& s);

	HRESULT CompileShaderFromFile(const wstring& szFileName, const string& szEntryPoint, const string& szShaderModel, ComPtr<ID3DBlob>& BlobOut);

	HRESULT CreateTextureFromFile(const wchar_t* szFileName, ID3D11ShaderResourceView** textureView, ID3D11Resource** texture = nullptr);

}


