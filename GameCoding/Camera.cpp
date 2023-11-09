#include "pch.h"
#include "Camera.h"

// static 변수 이기에 여기다가 실체를 구체화시켜준다.
Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera()
	: Super(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	// 카메라의 View 행렬 구하기 ---------------------------------
	// 방법 1
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	// 방법 2
	//S_MatView = GetTransform()->GetWorldMatrix().Invert();
	// -----------------------------------------------------------

	if (_type == ProjectionType::Perspective)
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
	else
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
}
