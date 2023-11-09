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

	// Transform 은 게임오브젝트마다 기본적으로 하나씩 가지고 있는 컴포넌트
	// 아래 함수는 게임오브젝트에 Transform 이 있으면 가져와주고 아니면 새로 추가해서 가져다준다.
	shared_ptr<Transform> GetOrAddTransform();

	void AddComponent(shared_ptr<Component> component);

private:
	ComPtr<ID3D11Device> _device;

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

