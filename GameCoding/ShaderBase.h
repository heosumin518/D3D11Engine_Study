#pragma once


enum ShaderScope
{
	SS_None = 0,
	SS_VertexShader = (1 << 0),		// 1	// bigflag 용도로 사용할 것이기에 비트 쉬프트 연산자를 사용하여 표현
	SS_PixelShader = (1 << 1),		// 2
	SS_Both = SS_VertexShader | SS_PixelShader,		// VertexShader 와 PixelShader 둘 다 사용
};

class ShaderBase
{
public:
	ShaderBase(ComPtr <ID3D11Device> device);
	virtual ~ShaderBase();

	virtual void Create(const wstring& path, const string& name, const string& version) abstract;

	ComPtr<ID3DBlob> GetBlob() { return _blob; }

protected:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);

protected:
	wstring _path;
	string _name;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3DBlob> _blob;
};

class VertexShader : public ShaderBase
{
	using Super = ShaderBase;	// Shader가 나의 부모 클래스
public:
	VertexShader(ComPtr<ID3D11Device> device);
	~VertexShader();

	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }
	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
};

class PixelShader : public ShaderBase
{
	using Super = ShaderBase;	
public:
	PixelShader(ComPtr<ID3D11Device> device);
	~PixelShader();

	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }
	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
};