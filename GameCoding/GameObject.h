#pragma once

class MonoBehaviour;
class Transform;
class Camera;
class MeshRenderer;
class Animator;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Camera> GetCamera();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Animator> GetAnimator();

	// Transform �� ���ӿ�����Ʈ���� �⺻������ �ϳ��� ������ �ִ� ������Ʈ
	// �Ʒ� �Լ��� ���ӿ�����Ʈ�� Transform �� ������ �������ְ� �ƴϸ� ���� �߰��ؼ� �������ش�.
	shared_ptr<Transform> GetOrAddTransform();

	void AddComponent(shared_ptr<Component> component);

private:
	ComPtr<ID3D11Device> _device;

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};
