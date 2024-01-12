#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ModelLoader.h"

namespace Engine
{
	class Model
	{
	public:
		Model();
		~Model();

	public:
		void Update(float deltaTime);
		void Render(ComPtr<ID3D11DeviceContext> deviceContext);

	public:
		vector<shared_ptr<Bone>> GetBones() { return m_bones; }
		vector<shared_ptr<Mesh>> GetMeshes() { return m_meshes; }
		vector<shared_ptr<Material>> GetMaterials() { return m_materials; }

	private:
		friend ModelLoader;

		shared_ptr<Node> m_rootNode;
		vector<shared_ptr<Node>> m_nodes;
		vector<shared_ptr<Bone>> m_bones;
		vector<shared_ptr<Mesh>> m_meshes;
		shared_ptr<Animation> m_animation;
		vector<shared_ptr<Material>> m_materials;
	};

}

