#pragma once

#include "Node.h"
#include "Mesh.h"
#include "Animation.h"
#include "Material.h"

class Model : public Node
{
public:
	Model() {}
	~Model() {}

	void Update(float deltaTime);

	bool ReadFile(ComPtr<ID3D11Device> device, const char* filePath);
	void UpdateNodeAnimationReference(UINT index);

	shared_ptr<Material> GetMaterial(UINT index);
	void SetWorldTransform(const Matrix& transform) { m_local = transform; };

private:
	vector<Mesh> m_meshes;
	vector<Material> m_materials;
	vector<Animation> m_animations;

	float m_animationProgressTime = 0.f;

};

