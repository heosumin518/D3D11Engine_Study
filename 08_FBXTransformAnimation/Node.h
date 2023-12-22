#pragma once
#include "ModelLoader.h"
#include "Struct.h"

class Node : public enable_shared_from_this<Node>
{
public:
	Node();
	~Node();

public:
	void Update();
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

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

	// hmm..
	//CB_ModelTransform m_CBNodeTransform;
	//ComPtr<ID3D11Buffer> m_nodeBuffer;	// TODO 231221 노드버퍼 정보를 넘겨주어야 함
};

