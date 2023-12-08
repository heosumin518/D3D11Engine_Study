#include "pch.h"
#include "Node.h"

#include "Animation.h"

Node::Node()
{
}

Node::~Node()
{
}

void Node::Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11Buffer> nodeBuffer)
{
	m_nodeBuffer = nodeBuffer;

	for (auto child : m_children)
	{
		if (child != nullptr)
			child->Init(device, nodeBuffer);
	}
}

void Node::Update(ComPtr<ID3D11DeviceContext> deviceContext, float deltaTime)
{
	Vector3 position;
	Vector3 scale;
	Quaternion rotation;
	
	// 노드 위치 곱하기
	{
		if (m_nodeAnimation != nullptr)
		{
			static float currentTime = 0.f;
			currentTime += deltaTime * 3;

			if (currentTime > m_nodeAnimation->frameCount)
				currentTime -= m_nodeAnimation->frameCount;

			// 애니메이션 보간
			InterpolateAnimationData(currentTime, position, scale, rotation);


		}
	}
}

void Node::InterpolateAnimationData(float currentTime, Vector3& outPosition, Vector3& outScaling, Quaternion& outRotation)
{
	// 애니메이션 데이터의 키프레임 개수를 가져옴
	unsigned int numFrames = m_nodeAnimation->frameCount;

	// 첫 번째 키프레임 데이터
	unsigned int frameIndex = 0;

	// 두 번째 키프레임 데이터
	unsigned int nextFrameIndex = 1;

	// 현재 시간을 가장 가까운 키프레임으로 보간
	for (unsigned int i = 0; i < numFrames - 1; ++i) {
		if (currentTime < m_nodeAnimation->keyFrame[nextFrameIndex].time) {
			break;
		}

		frameIndex = i;
		nextFrameIndex = i + 1;
	}

	// 현재 키프레임과 다음 키프레임의 시간 차이 계산
	float deltaTime = m_nodeAnimation->keyFrame[nextFrameIndex].time - m_nodeAnimation->keyFrame[frameIndex].time;

	// 만약 다음 키프레임과 현재 키프레임의 시간 차이가 없다면 1로 보정 ( 계산 오류로 애니메이션 보간 정보가 날아가는 경우 방지하기 위해 )
	if (deltaTime == 0)
		deltaTime = 1;

	// 현재 시간이 현재 키프레임과 다음 키프레임 사이의 비율 계산
	float factor = 0;
	if (m_nodeAnimation->keyFrame[frameIndex].time < currentTime)
		factor = (currentTime - m_nodeAnimation->keyFrame[frameIndex].time) / deltaTime;

	aiVector3D currentPosition;
	currentPosition.x = m_nodeAnimation->keyFrame[frameIndex].translation.x;
	currentPosition.y = m_nodeAnimation->keyFrame[frameIndex].translation.y;
	currentPosition.z = m_nodeAnimation->keyFrame[frameIndex].translation.z;

	aiVector3D currentScale;
	currentScale.x = m_nodeAnimation->keyFrame[frameIndex].scale.x;
	currentScale.y = m_nodeAnimation->keyFrame[frameIndex].scale.y;
	currentScale.z = m_nodeAnimation->keyFrame[frameIndex].scale.z;

	aiQuaternion currentRotation;
	currentRotation.x = m_nodeAnimation->keyFrame[frameIndex].rotation.x;
	currentRotation.y = m_nodeAnimation->keyFrame[frameIndex].rotation.y;
	currentRotation.z = m_nodeAnimation->keyFrame[frameIndex].rotation.z;
	currentRotation.w = m_nodeAnimation->keyFrame[frameIndex].rotation.w;

	aiVector3D nextPosition;
	nextPosition.x = m_nodeAnimation->keyFrame[nextFrameIndex].translation.x;
	nextPosition.y = m_nodeAnimation->keyFrame[nextFrameIndex].translation.y;
	nextPosition.z = m_nodeAnimation->keyFrame[nextFrameIndex].translation.z;

	aiVector3D nextScale;
	nextScale.x = m_nodeAnimation->keyFrame[nextFrameIndex].scale.x;
	nextScale.y = m_nodeAnimation->keyFrame[nextFrameIndex].scale.y;
	nextScale.z = m_nodeAnimation->keyFrame[nextFrameIndex].scale.z;

	aiQuaternion nextRotation;
	nextRotation.x = m_nodeAnimation->keyFrame[nextFrameIndex].rotation.x;
	nextRotation.y = m_nodeAnimation->keyFrame[nextFrameIndex].rotation.y;
	nextRotation.z = m_nodeAnimation->keyFrame[nextFrameIndex].rotation.z;
	nextRotation.w = m_nodeAnimation->keyFrame[nextFrameIndex].rotation.w;

	// 키프레임 데이터의 변환을 선형 보간하여 계산
	aiVector3D fianlPosition = currentPosition + factor * (nextPosition - currentPosition);
	aiVector3D fianlScale = currentScale + factor * (nextScale - currentScale);
	aiQuaternion fianlRotation;
	aiQuaternion::Interpolate(fianlRotation, currentRotation, nextRotation, factor);

	outPosition.x = fianlPosition.x;
	outPosition.y = fianlPosition.y;
	outPosition.z = fianlPosition.z;
	outScaling.x = fianlScale.x;
	outScaling.y = fianlScale.y;
	outScaling.z = fianlScale.z;
	outRotation.x = fianlRotation.x;
	outRotation.y = fianlRotation.y;
	outRotation.z = fianlRotation.z;
	outRotation.w = fianlRotation.w;
}

