#pragma once

class GameObject;
class Transform;

enum class ComponentType : uint8
{
	Transform,
	MeshRenderer,
	Camera,
	Animator,		// 고정적으로 하나 씩만 붙을 수 있는 컴포넌트
	// ...
	Script,			// 하나의 오브젝트에 여러개씩 붙을 수 있는 컴포넌트

	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1
};

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FixedUpdate() {}

public:
	ComponentType GetType() { return _type; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	ComponentType _type;
	weak_ptr<GameObject> _gameObject;
};

