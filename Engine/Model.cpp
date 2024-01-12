#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Bone.h"

Engine::Model::Model()
{
	
}

Engine::Model::~Model()
{
	
}

void Engine::Model::Update(float deltaTime)
{
	m_rootNode->Update(deltaTime);
}
