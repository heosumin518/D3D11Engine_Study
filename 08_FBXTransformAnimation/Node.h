#pragma once
#include "ModelLoader.h"
#include "Struct.h"

class Node
{
public:
	Node();
	~Node();

public:
	void Init(CB_Transform& nodeTransform, ComPtr<ID3D11Buffer> nodeBuffer,
		CB_UseTextureMap& matTransform, ComPtr<ID3D11Buffer> matBuffer, ComPtr<ID3D11BlendState> blendState);
	void Update();
	void Render(ComPtr<ID3D11DeviceContext> deviceContext, CB_Transform& nodeTransform);

	void SetLocalTransform(Matrix local) { m_matLocal = local; }

	friend ModelLoader;

private:
	string m_name;
	shared_ptr<Node> m_parent;
	vector<shared_ptr<Node>> m_children;
	vector<shared_ptr<Mesh>> m_meshs;
	Matrix m_matLocal;
	Matrix m_matWorld;
	shared_ptr<Model> m_owner;

	// 메인에 있는 트랜스폼 버퍼 가져와서 값 변경
	CB_Transform m_transform;
	ComPtr<ID3D11Buffer> m_buffer;
};

