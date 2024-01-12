#include "pch.h"
#include "Helper.h"

std::wstring Engine::ToWString(const std::string& s)
{
	std::wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}

HRESULT Engine::CompileShaderFromFile(const wstring& szFileName, const string& szEntryPoint,
	const string& szShaderModel, ComPtr<ID3DBlob>& BlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = ::D3DCompileFromFile(
		szFileName.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		szEntryPoint.c_str(),
		szShaderModel.c_str(),
		dwShaderFlags,
		0,
		BlobOut.GetAddressOf(),
		errorBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
		}
		return hr;
	}

	return S_OK;
}

HRESULT Engine::CreateTextureFromFile(const wchar_t* szFileName, ID3D11ShaderResourceView** textureView,
                                      ID3D11Resource** texture)
{
	std::filesystem::path path(szFileName);
	std::wstring strExtension = path.extension();
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);

	DirectX::TexMetadata metadata1;
	DirectX::ScratchImage scratchImage;

	HRESULT hr = S_OK;
	if (strExtension == L".dds")
	{
		CHECK(hr = DirectX::LoadFromDDSFile(szFileName, DirectX::DDS_FLAGS_NONE, &metadata1, scratchImage));
	}
	else if (strExtension == L".tga")
	{
		CHECK(hr = DirectX::LoadFromTGAFile(szFileName, &metadata1, scratchImage));
	}
	else if (strExtension == L".hdr")
	{
		CHECK(hr = DirectX::LoadFromHDRFile(szFileName, &metadata1, scratchImage));
	}
	else // ±‚≈∏..
	{
		CHECK(hr = DirectX::LoadFromWICFile(szFileName, DirectX::WIC_FLAGS_NONE, &metadata1, scratchImage));
	}

	CHECK(hr = DirectX::CreateShaderResourceView(DEVICE.Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, textureView));
	return hr;
}
