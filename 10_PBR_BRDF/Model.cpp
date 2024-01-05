#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Bone.h"

Model::Model()
{
	
}

Model::~Model()
{
	
}

void Model::Update(float deltaTime)
{
	m_rootNode->Update(deltaTime);
}
