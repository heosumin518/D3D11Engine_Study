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
	
	// ��� ��ġ ���ϱ�
	{
		if (m_nodeAnimation != nullptr)
		{
			static float currentTime = 0.f;
			currentTime += deltaTime * 3;

			if (currentTime > m_nodeAnimation->frameCount)
				currentTime -= m_nodeAnimation->frameCount;

			// �ִϸ��̼� ����
			InterpolateAnimationData(currentTime, position, scale, rotation);


		}
	}
}

void Node::InterpolateAnimationData(float currentTime, Vector3& outPosition, Vector3& outScaling, Quaternion& outRotation)
{
	// �ִϸ��̼� �������� Ű������ ������ ������
	unsigned int numFrames = m_nodeAnimation->frameCount;

	// ù ��° Ű������ ������
	unsigned int frameIndex = 0;

	// �� ��° Ű������ ������
	unsigned int nextFrameIndex = 1;

	// ���� �ð��� ���� ����� Ű���������� ����
	for (unsigned int i = 0; i < numFrames - 1; ++i) {
		if (currentTime < m_nodeAnimation->keyFrame[nextFrameIndex].time) {
			break;
		}

		frameIndex = i;
		nextFrameIndex = i + 1;
	}

	// ���� Ű�����Ӱ� ���� Ű�������� �ð� ���� ���
	float deltaTime = m_nodeAnimation->keyFrame[nextFrameIndex].time - m_nodeAnimation->keyFrame[frameIndex].time;

	// ���� ���� Ű�����Ӱ� ���� Ű�������� �ð� ���̰� ���ٸ� 1�� ���� ( ��� ������ �ִϸ��̼� ���� ������ ���ư��� ��� �����ϱ� ���� )
	if (deltaTime == 0)
		deltaTime = 1;

	// ���� �ð��� ���� Ű�����Ӱ� ���� Ű������ ������ ���� ���
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

	// Ű������ �������� ��ȯ�� ���� �����Ͽ� ���
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

