#pragma once

// Helper class for COM exceptions
class com_exception : public std::exception
{
public:
	com_exception(HRESULT hr) : result(hr) {}

	const char* what() const noexcept override
	{
		static char s_str[64] = {};
		sprintf_s(s_str, "Failure with HRESULT of %08X",
			static_cast<unsigned int>(result));
		return s_str;
	}

private:
	HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
inline void HR_T(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(const wstring& szFileName, const string& szEntryPoint, const string& szShaderModel, ComPtr<ID3DBlob>& BlobOut);
