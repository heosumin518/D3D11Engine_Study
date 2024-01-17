#pragma once
// FBX 파일로부터 정보를 로드하여 Model 객체에 넘겨주는 역할을 하는 클래스
// 리소스 매니저를 통해서만 사용이 되며 FBX 모델 데이터를 불러들여 리소스 매니저에 등록한다.

class Mesh;
class StaticMesh;
class SkeletalMesh;
class Node;
class Bone;
class Model;
class Material;
class Animation;
class NodeAnimation;

class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> m_device);
	~ModelLoader();
public:
	shared_ptr<Model> LoadModelFile(const string& file);
	void CreateNode(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> parent);

	void CreateMesh(aiNode* node, shared_ptr<Node> connectNode);	// TODO: 그냥 Mesh 없애고 밑에 두개로 대체
	shared_ptr<StaticMesh> CreateStaticMesh(aiNode* node, shared_ptr<Node> connectNode);
	shared_ptr<SkeletalMesh> CreateSkeletalMesh(aiNode* node, shared_ptr<Node> connectNode);

	void CreateMaterial();
	void CreateAnimation(aiAnimation* srcAnim);
	shared_ptr<NodeAnimation> ParseAnimationNode(shared_ptr<Animation> animation, aiNodeAnim* srcNodeAnim);

private:
	ComPtr<ID3D11Device> m_device;

private:
	shared_ptr<Assimp::Importer> m_importer;
	const aiScene* m_scene = nullptr;

private:
	// TODO: 아래 변수들은 모두 리소스 매니저가 가지고 있도록 하기. 그냥 가지고 있어도 될거 같기도 하고..? 흠
	vector<shared_ptr<Node>> m_nodes;
	vector<shared_ptr<Bone>> m_bones;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	shared_ptr<Animation> m_animation;
};

