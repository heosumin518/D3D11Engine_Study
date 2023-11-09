#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,    // 원근 투영
    Orthographic,   // 직교 투영
};

class Camera :
    public Component
{
    using Super = Component;
public:
    Camera();
    virtual ~Camera();

    virtual void Update() override;

    void SetProjectionType(ProjectionType type) { _type = type; }
    ProjectionType GetProjectionType() { return _type; }

    void UpdateMatrix();

private:
    ProjectionType _type = ProjectionType::Orthographic;

public:
    // 카메라를 전체 오브젝트 중 하나만 둔다라고 가정을 하고 view 와 projection을 들고있게 한다.
    static Matrix S_MatView;
    static Matrix S_MatProjection;
}; 

